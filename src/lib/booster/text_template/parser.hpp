#ifndef BOOSTER_TEXT_TEMPLATE_PARSER_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_PARSER_HPP_INCLUDED

#include <booster/text_template/error.hpp>
#include <booster/text_template/text_node.hpp>
#include <booster/text_template/text_template.hpp>

namespace booster {
    namespace text_template {
        
        // =====================================================================
        // Function Declarations
        // =====================================================================
        
        template<typename I>
        bool parse(I& begin, I end, text_template& tpl,
                   boost::system::error_condition& e);
        
        template<typename I>
        text_template parse(I& begin, I end);
        
        // =====================================================================
        // Interface
        // =====================================================================
        
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
                        //!\todo Finish parsing of quick print!
                        parse_quick_print_open(begin, end);
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
                    iterator it2(it);
                    if (parse_quick_print_open(it2, end)) {
                        parent.children_->push_back(new text_node(text));
                        it = it2;
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
            
            //! \todo What about the trailing whitespace after the 't'?
            if (parse_sequence(it, end, "@print") &&
                parse_alternates(it, end, whitespace())) {
                
                begin = it;
                return true;
            }
            
            return false;
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
        bool parse(I& begin, I end, text_template& tpl,
                   boost::system::error_condition& e) {
            parser<I> p;
            return p.parse(begin, end, tpl, e);
        }
        
        template<typename I>
        text_template parse(I& begin, I end) {
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
