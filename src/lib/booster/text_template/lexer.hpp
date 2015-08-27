#ifndef BOOSTER_TEXT_TEMPLATE_LEXER_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_LEXER_HPP_INCLUDED

#include <booster/text_template/common.hpp>
#include <booster/text_template/token.hpp>
#include <string>
#include <vector>

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
            
            token get_text(iterator& it, iterator end);
            token get_token(iterator& it, iterator end);
            
            token lex_double_value(iterator& it, iterator end, const input_position& pos);
            
            // -----------------------------------------------------------------
            // Variables
            // -----------------------------------------------------------------
            
        private:
            
            bool lex_sequence(iterator& begin, iterator end,
                              const std::string& chars);
            
            bool get_char(char& c, iterator& it, iterator end);
            token position_to_token(const input_position& p, symbol_type s, const string_type& value);
            
            input_position position_;
            
            typedef std::vector<token> cache_type;
            cache_type cache_;
            
            typedef std::vector<char> char_cache_type;
            char_cache_type ccache_;
            
            //! \todo A clear() method to clear cache and position?
            
            bool is_whitespace(char c) {
                return ' ' == c || '\t' == c || '\n' == c || '\r' == c;
            }
            
            bool is_digit(char c) {
                return '0' <= c && '9' >= c;
            }
            
            void unget_char(char c);
            
            bool lex_alternates(iterator& begin, iterator end,
                                          const std::string& chars);
            
            
        };
        
        // =====================================================================
        // Implementation
        // =====================================================================
        
        //! \todo Fix the filename.
        template<typename I>
        inline lexer<I>::lexer() : position_("unknown", 1, 1, 1) {}

        template<typename I>
        token lexer<I>::get_text(iterator& it, iterator end) {
            if (cache_.size()) {
                token tk = cache_.back();
                cache_.pop_back();
                return tk;
            }
            
            if (it == end) {
                return token(position_, ts_eof, "");
            }
            
            input_position p(position_);
            string_type text;
            char c;

            while (get_char(c, it, end)) {
                
                //! \todo Fix this
                if ('@' == c) {
                    input_position print_pos(position_);
                    iterator next(it);
                    if (lex_sequence(next, end, "print") && next != end &&
                        (is_whitespace(*next) || '(' == *next)) {
                        cache_.push_back(token(print_pos, ts_quick_print, "@print"));
                        return token(position_, ts_text, text);
//                        char n;
//                        if (!get_char(n, next, end) || !is_whitespace(n) || '(') {
//                            // TODO: This should unget char.
//                        }
                    }
                }
                
                text += c;
            }
            
            return position_to_token(p, ts_text, text);
        }
        
        template<typename I>
        token lexer<I>::get_token(iterator& it, iterator end) {
            if (cache_.size()) {
                token tk = cache_.back();
                cache_.pop_back();
                return tk;
            }
            
            if (it == end) {
                return token(position_, ts_eof, "");
            }
            
            iterator prev(it);
            char c;
            token tk;
            
            input_position p(position_);
            while (get_char(c, it, end)) {
                if ('+' == c || '-' == c || is_digit(c)) {
                    if ((tk = lex_double_value(prev, end, p))) {
                        it = prev;
                        return tk;
                    }
                    //! \todo parse of integer.
                }
                //! \todo Other parsers.
            }
            
            return token();
        }
        
        template<typename I>
        inline bool lexer<I>::get_char(char& c, iterator& it, iterator end) {
            if (ccache_.size()) {
                c = ccache_.back();
                ccache_.pop_back();
                return true;
            }
            
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
        
        template<typename I>
        inline bool lexer<I>::lex_sequence(iterator& it, iterator end,
                                           const std::string& chars) {
            iterator next(it);
            std::string::const_iterator sit = chars.begin(), send = chars.end();
            
            char c;
            while (get_char(c, next, end)) {
                if (c != *sit) {
                    return false;
                }
                
                ++sit;
                
                if (sit == send) {
                    it = next;
                    return true;
                }
            }
            
            return false;
        }
        
        template<typename I>
        inline token lexer<I>::lex_double_value(iterator& begin, iterator end, const input_position& p) {
            if (begin == end) {
                return token();
            }
            
            input_position pos(position_);
            iterator it(begin), prev(it);
            std::string result, digits = "0123456789";
            unsigned i = 0;
            bool is_negative = false, found_sign = false, found_dot = false;
            char c;
            
            while (get_char(c, it, end)) {
                if (0 == i) {
                    if ('-' == c) {
                        is_negative = true;
                        found_sign = true;
                        result += c;
                    } else if ('+' == c) {
                        found_sign = true;
                        result += c;
                    } else {
                        if (!is_digit(c)) {
                            return token();
                        } else {
                            result += c;
                        }
                    }
                } else {
                    if ('.' == c) {
                        if ('-' == result[i-1] || '+' == result[i-1]) {
                            return token();
                        } else if ('.' == result[i-1]) {
                            if (found_dot) {
                                unget_char(c);
                                it = prev;
                                break;
                            } else {
                                found_dot = true;
                                result += c;
                            }
                        } else {
                            found_dot = true;
                            result += c;
                        }
                    } else {
                        if (!is_digit(c)) {
                            if ('-' == result[i-1] || '+' == result[i-1]) {
                                return token();
                            } else if ('.' == result[i-1]) {
                                return token();
                            } else {
                                ++i;
                                break;
                            }
                        } else {
                            result += c;
                        }
                    }
                }
                
                prev = it;
                ++i;
            }

            if (!found_dot) {
                return token();
            }
            
            if ('.' == result[i-1]) {
                return token();
            }
            
            try {
                boost::lexical_cast<double_type>(result);
                begin = it;
                return token(pos, ts_double, result);
            } catch (boost::bad_lexical_cast&) {
                return token();
            }
        }
        
        template<typename I>
        void lexer<I>::unget_char(char c) {
            ccache_.push_back(c);
        }
        
        
        //! \todo Implement unget_char. Perhaps using a cache?
        
        template<typename I>
        bool lexer<I>::lex_alternates(iterator& begin, iterator end,
                                         const std::string& chars) {
            if (begin == end) {
                return false;
            }
            
            //! \todo Use get_char
            
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
        
    }
}


#endif // #ifndef BOOSTER_TEXT_TEMPLATE_LEXER_HPP_INCLUDED
