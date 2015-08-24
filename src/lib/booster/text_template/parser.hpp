#ifndef BOOSTER_TEXT_TEMPLATE_PARSER_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_PARSER_HPP_INCLUDED

#include <boost/lexical_cast.hpp>
#include <booster/text_template/error.hpp>
#include <booster/text_template/quick_print_node.hpp>
#include <booster/text_template/text_node.hpp>
#include <booster/text_template/text_template.hpp>
#include <booster/text_template/value_node.hpp>

namespace booster {
    namespace text_template {
        
        // =====================================================================
        // Function Declarations
        // =====================================================================
        
        template<typename I>
        bool parse(I begin, I end, text_template& tpl,
                   boost::system::error_condition& e);
        
        template<typename I>
        text_template parse(I begin, I end);
        
        // =====================================================================
        // Interface
        // =====================================================================

        /**
         
         \todo Document the following ABNF-style syntax.
         
         condition  = expression [< <= == != >= > ] expression
         expression = [+ -] term ([+ -] term)
         ; one or more
         term       = factor [* /] factor
         factor     = identifier | value | "(" expression ")"
         identifier = [a-zA-Z_][a-zA-Z_0-9]*
         ; what about dots and subscripts for traversal?
         value      = integer | double | string | object | array
         ; object and array in JSON-like notation.
         
         */
        

        
        /*!
         \brief A parser transforms a series of characters (of type char)
         into a parse tree for execution.
         
         A parser operates on iterators (the \p I template parameter). This
         allows parsers to work with different containers.
         
         \todo Make a note about pass-by-reference for the begin iterator.
         */
        template<typename I>
        class parser {
        private:
            
            // -----------------------------------------------------------------
            // Typedefs
            // -----------------------------------------------------------------
            
            //! \todo These typedefs may depend on climits or some boot macro
            //        that indicates the maximum value for these.
            typedef long double double_type;
            typedef long long int integer_type;
            
            // -----------------------------------------------------------------
            // Lookaheads
            // -----------------------------------------------------------------
            
            typedef int lookahead;
            static const lookahead lookahead_quick_print_open      = 0x00;
            static const lookahead lookahead_eof                   = 0x01;
            
        public:
            
            // -----------------------------------------------------------------
            // Typedefs
            // -----------------------------------------------------------------
            
            typedef I iterator;
            
            // -----------------------------------------------------------------
            // High-level Parsing
            // -----------------------------------------------------------------
            
            /*!
             \brief The parse() method is the main method used to parse input
             in order to produce a parse tree.
             */
            bool parse(iterator& begin, iterator end, text_template& tpl,
                       boost::system::error_condition& e);
            
            // -----------------------------------------------------------------
            // Low-level Parsing
            // -----------------------------------------------------------------
            
            /*!
             \brief The parse_text() method parses a block of text.
             
             This method has no return value and no pass-by-reference to an
             error_condition because it always succeeds, even on empty input.
             */
            lookahead parse_text(iterator& begin, iterator end,
                                 parent_node& parent);
            
            /*!
             \brief The parse_quick_print_open() method parses the opening quick
             print instruction (e.g., "@print").
             */
            bool parse_quick_print_open(iterator& begin, iterator end);
            
            /*!
             \brief The parse_quick_print() method parses a quick-print block.
             
             Note that a "@print" string must have already been encountered.
             */
            bool parse_quick_print(iterator& begin, iterator end,
                                   parent_node& parent);
            
            /*!
             \brief The parse_comma_separated_conditions() method does as its
             name suggests.
             */
            bool parse_comma_separated_conditions(iterator& begin,
                                                  iterator end,
                                                  parent_node& parent);
            
            /*!
             \brief A condition is a test between two expressions (e.g., 1 < 2).
             
             Each side of the condition can be a full expression (e.g., a literal
             value, a multiplication, etc.
             
             condition  = expression [< <= == != >= > ] expression
             
             \brief Implement
             */
            bool parse_condition(iterator& begin, iterator end, parent_node& parent);
            
            /*!
             \todo Implement
             */
            bool parse_expression(iterator& begin, iterator end, parent_node& parent);
            /*!
             \todo Implement
             */
            bool parse_term(iterator& begin, iterator end, parent_node& parent);
            /*!
             \todo Implement
             */
            bool parse_factor(iterator& begin, iterator end, parent_node& parent);
            /*!
             \todo Implement
             */
            bool parse_identifier(iterator& begin, iterator end, parent_node& parent);
            /*!
             \todo Implement
             */
            bool parse_value(iterator& begin, iterator end, parent_node& parent);
            
            /*!
             \todo Implement
             */
            bool parse_double_value(iterator& begin, iterator end, parent_node& parent);
            
            /*!
             \todo Implement
             */
            bool parse_integer_value(iterator& begin, iterator end, parent_node& parent);
            
            /*!
             \todo Implement
             */
            bool parse_string_value(iterator& begin, iterator end, parent_node& parent);
            /*!
             \todo Implement
             */
            bool parse_object_value(iterator& begin, iterator end, parent_node& parent);
            /*!
             \todo Implement
             */
            bool parse_array_value(iterator& begin, iterator end, parent_node& parent);
            
            // -----------------------------------------------------------------
            // Parsing Helpers
            // -----------------------------------------------------------------
            
            /*!
             \brief The parse_sequence() method parses a sequence of characters.
             */
            bool parse_sequence(iterator& begin, iterator end,
                                const std::string& chars);
            
            /*!
             \brief The parse_alternates() method parses any of the characters.
             */
            bool parse_alternates(iterator& begin, iterator end,
                                const std::string& chars);
            
            // -----------------------------------------------------------------
            // Character Classes
            // -----------------------------------------------------------------
            
            std::string whitespace() {
                return " \t\r\n";
            }
            
            std::string digits() {
                return "0123456789";
            }
            
        };
        
        // =====================================================================
        // Implementation
        // =====================================================================
        
        // ---------------------------------------------------------------------
        // High-level Parsing
        // ---------------------------------------------------------------------
        
        template<typename I>
        inline bool parser<I>::parse(iterator& begin, iterator end,
                                     text_template& tpl,
                                     boost::system::error_condition& e) {
            
            while (begin != end) {
                lookahead l = parse_text(begin, end, *tpl.nodes_);
                switch (l) {
                    case lookahead_quick_print_open:
                        parse_quick_print(begin, end, *tpl.nodes_);
                        break;
                    case lookahead_eof:
                    default:
                        return true;
                }
                
                //! \todo Finish
            }
            
            return true;
        }
        
        // ---------------------------------------------------------------------
        // Low-level Parsing
        // ---------------------------------------------------------------------
        
        template<typename I>
        inline typename parser<I>::lookahead parser<I>::parse_text(iterator& begin,
                                                          iterator end,
                                                          parent_node& parent) {
            
            iterator it(begin);
            if (it == end) {
                return lookahead_eof;
            }
            
            std::string text;
            
            while (it != end) {
                if (*it == '@') {
                    iterator next(it);
                    if (parse_quick_print_open(next, end)) {
                        parent.children_->push_back(new text_node(text));
                        it = next;
                        return lookahead_quick_print_open;
                    } else {
                        text += *it;
                    }
                } else {
                    text += *it;
                }
                
                ++it;
            }
            
            begin = it;
            parent.children_->push_back(new text_node(text));
            return lookahead_eof;
        }
        
        template<typename I>
        inline bool parser<I>::parse_quick_print_open(iterator& begin,
                                                      iterator end) {
            iterator it(begin);
            
            if (!parse_sequence(it, end, "@print")) {
                return false;
            }
            
            iterator next(it);
            if (!parse_alternates(next, end, whitespace() + "(")) {
                return false;
            }
            
            begin = it;
            return true;
        }
        
        template<typename I>
        inline bool parser<I>::parse_quick_print(iterator& begin,
                                                 iterator end,
                                                 parent_node& parent) {
            
            iterator it(begin);
            quick_print_node* qprint = new quick_print_node();
            parent.children_->push_back(qprint);
            
            //! \todo parse_comma_separated_expressions
            return false;
        }
        
        template<typename I>
        bool parser<I>::parse_comma_separated_conditions(iterator& begin,
                                                         iterator end,
                                                         parent_node& parent) {
            
            //! \todo Finish
            return false;
        }
        
        template<typename I>
        bool parser<I>::parse_double_value(iterator& begin,
                                           iterator end,
                                           parent_node& parent) {
            if (begin == end) {
                return false;
            }
            
            iterator it(begin), prev(it);
            std::string result, digs = digits();
            unsigned i = 0;
            bool is_negative = false, found_sign = false, found_dot = false;
            
            while (it != end) {
                if (0 == i) {
                    if (*it == '-') {
                        is_negative = true;
                        found_sign = true;
                        result += *it;
                    } else if (*it == '+') {
                        found_sign = true;
                        result += *it;
                    } else {
                        iterator next(it);
                        if (!parse_alternates(next, end, digs)) {
                            return false;
                        } else {
                            result += *it;
                        }
                    }
                } else {
                    if (*it == '.') {
                        if (result[i-1] == '-' || result[i-1] == '+') {
                            return false;
                        } else if (result[i-1] == '.') {
                            if (found_dot) {
                                it = prev;
                                break;
                            } else {
                                found_dot = true;
                                result += *it;
                            }
                        } else {
                            found_dot = true;
                            result += *it;
                        }
                    } else {
                        iterator next(it);
                        if (!parse_alternates(next, end, digs)) {
                            if (result[i-1] == '-' || result[i-1] == '+') {
                                return false;
                            } else {
                                break;
                            }
                        } else {
                            result += *it;
                        }
                    }
                }
                
                prev = it;
                ++i;
                ++it;
            }
            
            if (!found_dot) {
                return false;
            }
            
            if (result[i-1] == '.') {
                return false;
            }
            
            try {
                double_type d = boost::lexical_cast<double_type>(result);
                parent.children_->push_back(new value_node<double_type>(d));
                begin = it;
                return true;
            } catch (boost::bad_lexical_cast&) {
                return false;
            }
        }
        
        template<typename I>
        bool parser<I>::parse_integer_value(iterator& begin,
                                            iterator end,
                                            parent_node& parent) {
            if (begin == end) {
                return false;
            }
            
            iterator it(begin), prev(it);
            std::string result, digs = digits();
            unsigned i = 0;
            bool is_negative = false, found_sign = false;
            
            while (it != end) {
                if (0 == i) {
                    if (*it == '-') {
                        is_negative = true;
                        found_sign = true;
                        result += *it;
                    } else if (*it == '+') {
                        found_sign = true;
                        result += *it;
                    } else {
                        iterator next(it);
                        if (!parse_alternates(next, end, digs)) {
                            return false;
                        } else {
                            result += *it;
                        }
                    }
                } else {
                    iterator next(it);
                    if (!parse_alternates(next, end, digs)) {
                        if (result[i-1] == '-' || result[i-1] == '+') {
                            return false;
                        } else {
                            break;
                        }
                    } else {
                        result += *it;
                    }
                }
                
                prev = it;
                ++i;
                ++it;
            }
            
            try {
                integer_type i = boost::lexical_cast<integer_type>(result);
                parent.children_->push_back(new value_node<integer_type>(i));
                begin = it;
                return true;
            } catch (boost::bad_lexical_cast&) {
                return false;
            }
        }
        
        // ---------------------------------------------------------------------
        // Parsing Helpers
        // ---------------------------------------------------------------------
        
        template<typename I>
        inline bool parser<I>::parse_sequence(iterator& begin, iterator end,
                                              const std::string& chars) {
            iterator it(begin);
            
            for (std::string::const_iterator sit = chars.begin(),
                 send = chars.end();
                 sit != send; ++sit) {
                
                if (it == end) {
                    return false;
                }
                
                if (*it != *sit) {
                    return false;
                }
                
                ++it;
            }
            
            begin = it;
            return true;
        }
        
        template<typename I>
        bool parser<I>::parse_alternates(iterator& begin, iterator end,
                                         const std::string& chars) {
            if (begin == end) {
                return false;
            }
            
            for (std::string::const_iterator sit = chars.begin(),
                 send = chars.end();
                 sit != send; ++sit) {
                
                if (*begin == *sit) {
                    ++begin;
                    return true;
                }
            }
            
            return false;
        }
        
        // =====================================================================
        // Function Definitions
        // =====================================================================

        template<typename I>
        bool parse(I begin, I end, text_template& tpl,
                   boost::system::error_condition& e) {
            parser<I> p;
            return p.parse(begin, end, tpl, e);
        }
        
        template<typename I>
        text_template parse(I begin, I end) {
            text_template tpl;
            boost::system::error_condition e;
            parser<I> p;
            
            if (!p.parse(begin, end, tpl, e)) {
                throw boost::system::system_error(e.value(), e.category());
            }
            
            return tpl;
        }
        
    }
}

#endif // #ifndef BOOSTER_TEXT_TEMPLATE_PARSER_HPP_INCLUDED
