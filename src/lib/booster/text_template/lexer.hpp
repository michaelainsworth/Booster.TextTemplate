#ifndef BOOSTER_TEXT_TEMPLATE_LEXER_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_LEXER_HPP_INCLUDED

#include <booster/text_template/common.hpp>
#include <booster/text_template/token.hpp>
#include <string>

namespace booster {
    namespace text_template {
        
        // =====================================================================
        // Interface
        // =====================================================================
        
        /*!
         The lexer class is used to transform a series of input characters into
         a series of tokens.
         */
        template<typename I>
        class lexer {
        public:
            
            // -----------------------------------------------------------------
            // Typedefs
            // -----------------------------------------------------------------
            
            typedef std::string string_type;
            //! \todo Change to iterator_type?
            typedef I iterator;

            // -----------------------------------------------------------------
            // Lifecycle
            // -----------------------------------------------------------------
            
            lexer();
            
            // -----------------------------------------------------------------
            // Lexing methods
            // -----------------------------------------------------------------
            
            token get_token(iterator& it, iterator end);
            
            // -----------------------------------------------------------------
            // Variables
            // -----------------------------------------------------------------
            
        private:
            
            bool get_char(char& c, iterator& it, iterator end);
            token position_to_token(const input_position& p, symbol_type s, const string_type& value);
            
            input_position position_;
            
        };
        
        // =====================================================================
        // Implementation
        // =====================================================================
        
        //! \todo Fix the filename.
        template<typename I>
        inline lexer<I>::lexer() : position_("unknown", 1, 1, 1) {}

        template<typename I>
        token lexer<I>::get_token(iterator& it, iterator end) {
            if (it == end) {
                return token();
            }
            
            input_position p(position_);
            string_type text;
            char c;

            while (get_char(c, it, end)) {
                
                //! \todo Fix this
//                if (*it == '@') {
//                    iterator next(it);
//                    if (parse_quick_print_open(next, end)) {
//                        parent.children_->push_back(new text_node(text));
//                        it = next;
//                        return lookahead_quick_print_open;
//                    } else {
//                        text += *it;
//                    }
//                }
                
                text += c;
            }
            
            return position_to_token(p, ts_text, text);
        }
        
        template<typename I>
        inline bool lexer<I>::get_char(char& c, iterator& it, iterator end) {
            if (it == end) {
                return false;
            }
            
            c = *it;
            if (c == '\r' || c == '\n') {
                ++position_.line;
                position_.column = 1;
            } else {
                ++position_.column;
            }
            
            ++position_.offset;
            ++it;
            
            return true;
        }
        
        template<typename I>
        inline token lexer<I>::position_to_token(const input_position& p, symbol_type s, const string_type& value) {
            return token(p, s, value);
        }
        
    }
}


#endif // #ifndef BOOSTER_TEXT_TEMPLATE_LEXER_HPP_INCLUDED
