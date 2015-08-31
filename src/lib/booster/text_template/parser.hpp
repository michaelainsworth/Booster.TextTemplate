#ifndef BOOSTER_TEXT_TEMPLATE_PARSER_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_PARSER_HPP_INCLUDED

#include <boost/lexical_cast.hpp>
#include <booster/text_template/call_node.hpp>
#include <booster/text_template/error.hpp>
#include <booster/text_template/lexer.hpp>
#include <booster/text_template/symbol_type.hpp>
#include <booster/text_template/text_node.hpp>
#include <booster/text_template/text_template.hpp>
#include <booster/text_template/token.hpp>
#include <booster/text_template/value_node.hpp>
#include <map>
#include <stack>

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
        
        /*!
         \brief A parser transforms a series of characters (of type char)
         into a parse tree for execution.
         
         A parser operates on iterators (the \p I template parameter). This
         allows parsers to work with different containers.
         
         \todo Make a note about pass-by-reference for the begin iterator.
         
         \todo Previously this was a recursive descent parser - it's now going
         to be an LL parser.
         
            program             = ?

            text                = ?
            instruction         = quick_print | quick_for
            quick_print         = "@print " argument_list ";"
            quick_for           = "@for " "(" "auto" identifier ":" logical ")" ":"
            (text | instruction)
            "@endfor;"
            argument_list       = logical *("," logical)
            logical             = comparison *1(("&&" | "||") comparison)
            comparison          = expression *1(("<" | "<=" | "==" | "!=" | ">=" | ">") expression)
            expression          = term *1(("+" | "-") term)
            term                = factor *1("*" | "/") factor)
            factor              = value | reference | "(" logical ")"
            reference           = identifier *1(resolver)
            resolver            = ( "[" logical "]" ) | ( "." identifier )
            identifier          = [_a-zA-Z][_a-zA-Z0-9]*
            value               = "true" | "false" | double | integer | string | array | dictionary
            double              = [+-]?[0-9]+(\.[0-9]+)
            integer             = [+-]?[0-9]+
            string              = '"' *char '"'
            array               = "[" *1(logical) "]"
            dictionary          = "{" *1(identifier ":" logical)
         */
        template<typename I>
        class parser {
        private:
            
            // -----------------------------------------------------------------
            // Typedefs
            // -----------------------------------------------------------------
            
            typedef void (parser::*parse_function_type)(const token& tk);
            typedef std::map<symbol_type, parse_function_type> ts_function_map;
            typedef std::map<symbol_type, ts_function_map> nts_function_map;
            typedef std::stack<symbol_type> stack_type;
            typedef I iterator;
            typedef std::string string_type;
            typedef lexer<I> lexer_type;
            typedef token (lexer_type::*lexer_function)(iterator& it, iterator end);
            typedef std::stack<node*> node_stack_type;
            
            // -----------------------------------------------------------------
            // Lookaheads
            // -----------------------------------------------------------------
            
            typedef int lookahead;
            static const lookahead lookahead_quick_print_open      = 0x00;
            static const lookahead lookahead_eof                   = 0x01;
            
        public:
            
            // -----------------------------------------------------------------
            // Lifecycle
            // -----------------------------------------------------------------
            
            parser();
            
            // -----------------------------------------------------------------
            // High-level Parsing
            // -----------------------------------------------------------------
            
            /*!
             \brief The parse() method is the main method used to parse input
             in order to produce a parse tree.
             */
            bool parse(const string_type& filename, iterator& begin, iterator end, text_template& tpl,
                       boost::system::error_condition& e);
            
            // -----------------------------------------------------------------
            // Low-level Parsing
            // -----------------------------------------------------------------
            
            /*!
             \brief The parse_quick_print_open() method parses the opening quick
             print instruction (e.g., "@print").
             */
            bool parse_quick_print_open(iterator& begin, iterator end);
            
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
            bool parse_integer_value(iterator& begin, iterator end, parent_node& parent);
            
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

            //! \todo Remove
            std::string whitespace() {
                return " \t\r\n";
            }

            //! \todo Remove
            std::string digits() {
                return "0123456789";
            }
            
            void parse_text(const token& tk) {
                //! \todo Nodes should have file, line, column and offset.
                parent_node* p = dynamic_cast<parent_node*>(nstack_.top());
                p->children_->push_back(new text_node(tk.value));
                lex_ = &lexer_type::get_token;
            }
            
            void parse_eof(const token& tk) {
                //! \todo What to do here?
                stack_.pop();
                stack_.pop();
            }
            
            void parse_quick_print(const token& tk) {
                stack_.push(nts_quick_print);
                parent_node* p = dynamic_cast<parent_node*>(nstack_.top());
                call_node* c = new call_node(tk.position);
                p->children_->push_back(c);
                nstack_.push(c);
            }
            
            void parse_quick_print_head(const token& tk) {
                double_type d = boost::lexical_cast<double_type>(tk.value);
                //! \todo Create a function called top_as<parent_node> which
                //! \todo performs the cast.
                parent_node* p = dynamic_cast<parent_node*>(nstack_.top());
                p->children_->push_back(new value_node<double_type>(d));
                stack_.pop();
                stack_.push(nts_quick_print_tail);
            }
            
            void parse_quick_print_tail(const token& tk) {
                if (ts_semicolon == tk.type) {
                    stack_.pop();
                    nstack_.pop();
                    lex_ = &lexer_type::get_text;
                } else {
                    //! \todo When a comma, just consume?
                }
            }
            
            // -----------------------------------------------------------------
            // Variables
            // -----------------------------------------------------------------
            
        private:
            
            nts_function_map table_;
            lexer_type lexer_;
            stack_type stack_;
            node_stack_type nstack_;
            lexer_function lex_;
            
        };
        
        // =====================================================================
        // Implementation
        // =====================================================================

        // ---------------------------------------------------------------------
        // Lifecycle
        // ---------------------------------------------------------------------
        
        template<typename I>
        parser<I>::parser() {
            //! \todo Set up the parse table.
            table_[nts_template][ts_text] = &parser<I>::parse_text;
            table_[nts_template][ts_quick_print] = &parser<I>::parse_quick_print;
            table_[nts_template][ts_eof] = &parser<I>::parse_eof;
            
            table_[nts_quick_print][ts_double] = &parser<I>::parse_quick_print_head;
            table_[nts_quick_print_tail][ts_comma] = &parser<I>::parse_quick_print_tail;
            table_[nts_quick_print_tail][ts_semicolon] = &parser<I>::parse_quick_print_tail;
            
        }
        
        // ---------------------------------------------------------------------
        // High-level Parsing
        // ---------------------------------------------------------------------
        
        template<typename I>
        inline bool parser<I>::parse(const string_type& filename, iterator& it, iterator end,
                                     text_template& tpl,
                                     boost::system::error_condition& e) {
            
            lex_ = &lexer_type::get_text;
            lexer_.reset(filename);
            while (stack_.size()) { stack_.pop(); }
            
            while (nstack_.size()) {
                node* n = nstack_.top();
                delete n;
                nstack_.pop();
            }
            
            nstack_.push(new parent_node(lexer_.get_position()));
            
            stack_.push(ts_eof);
            stack_.push(nts_template);
            
            typename nts_function_map::iterator
                  nts_end = table_.end()
                , nts_it = nts_end;
            
            while (true) {
                symbol_type state = stack_.top();
                
                token tk = (lexer_.*lex_)(it, end);
                if (!tk) {
                    //! \todo convert this to use error_conditions.
                    //! \todo Create two function make_error_condition()
                    //! \todo and throw_system_error().
                    //! \todo According to the current symbol stack, this
                    //! \todo ... could say what's expected.
                    e = boost::system::error_condition(terminal_unexpected,
                                                       get_error_category());
                    return false;
                }
                
                nts_it = table_.find(state);
                if (nts_it == nts_end) {
                    e = boost::system::error_condition(non_terminal_unexpected,
                                                       get_error_category());
                    return false;
                }
                
                typename ts_function_map::iterator
                ts_it = nts_it->second.find(tk.type)
                , ts_end = nts_it->second.end();
                
                if (ts_it == ts_end) {
                    //! \todo change error message to "don't know how to process that."
                    e = boost::system::error_condition(terminal_unexpected,
                                                       get_error_category());
                    return false;
                }
                
                parse_function_type pf = ts_it->second;
                (this->*pf)(tk);
                
                if (!stack_.size()) {
                    parent_node* p = dynamic_cast<parent_node*>(nstack_.top());
                    tpl.nodes_ = text_template::parent_node_ptr(p);
                    while (nstack_.size()) {
                        nstack_.pop();
                    }
                    break;
                }
            }
            
            return true;
        }
        
        // ---------------------------------------------------------------------
        // Low-level Parsing
        // ---------------------------------------------------------------------
        
        template<typename I>
        bool parser<I>::parse_comma_separated_conditions(iterator& begin,
                                                         iterator end,
                                                         parent_node& parent) {
            
            //! \todo Finish
            return false;
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
            //! \todo Fix filename
            return p.parse("unknown", begin, end, tpl, e);
        }
        
        template<typename I>
        text_template parse(I begin, I end) {
            text_template tpl;
            boost::system::error_condition e;
            parser<I> p;
            
            //! \todo Fix filename!
            if (!p.parse("unknown", begin, end, tpl, e)) {
                throw boost::system::system_error(e.value(), e.category());
            }
            
            return tpl;
        }
        
    }
}

#endif // #ifndef BOOSTER_TEXT_TEMPLATE_PARSER_HPP_INCLUDED
