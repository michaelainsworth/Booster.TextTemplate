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
            
            token lex_double(iterator& it, iterator end, const input_position& pos);
            
            input_position get_position() const {
                return position_;
            }
            
            // -----------------------------------------------------------------
            // Variables
            // -----------------------------------------------------------------
            
            void reset(const string_type& filename) {
                position_ = input_position(filename, 1, 1, 1);
                ccache_.clear();
                cache_.clear();
            }
            
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
            
            
            
            
            
            void unget_char(char c);
            
            bool lex_alternates(iterator& begin, iterator end,
                                          const std::string& chars);
            
            
        };
        
        //! \todo Templatise
        inline bool is_digit(char c) {
            return '0' <= c && '9' >= c;
        }
        
        
        
        //! \todo Templatize
        inline bool is_lower(char c) {
            return c >= 'a' && c <= 'z';
        }
        
        //! \todo Templatize
        inline bool is_upper(char c) {
            return c >= 'A' && c <= 'Z';
        }
        
        //! \todo Templatize
        inline bool is_alpha(char c) {
            return is_lower(c) || is_upper(c);
        }
        
        //! \todo Templatize
        inline bool is_punctuation(char c) {
            return std::string("`-=~!@#$%^&*()_+[]\{}|;':\",./<>?").find(c) != std::string::npos;
        }

        //! \todo Templatize
        inline bool is_printable(char c) {
            return is_alpha(c) || is_digit(c) || is_punctuation(c);
        }
        
        //! \todo Templatize
        inline bool is_whitespace(char c) {
            return ' ' == c || '\t' == c || '\n' == c || '\r' == c;
        }
        
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
                if ('@' == c) {
                    input_position print_pos(position_);
                    iterator next(it);
                    if (lex_sequence(next, end, "print") && next != end) {
                        if ((is_whitespace(*next) || '(' == *next)) {
                            it = next;
                            cache_.push_back(token(print_pos, ts_quick_print, "@print"));
                            return token(position_, ts_text, text);
                        }
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
                    if ((tk = lex_double(prev, end, p))) {
                        it = prev;
                        return tk;
                    } else {
                        //! \todo parse of integer, plus, minus.
                        return token();
                    }
                } else if (',' == c) {
                    return token(p, ts_comma, c);
                } else if (';' == c) {
                    return token(p, ts_semicolon, c);
                } else if (is_whitespace(c)) {
                    // Skip whitespace.
                } else {
                    return token(p, ts_unknown, c);
                }
                
                prev = it;
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
        inline token lexer<I>::lex_double(iterator& begin, iterator end, const input_position& p) {
            if (begin == end) {
                return token();
            }
            
            input_position pos(position_);
            iterator it(begin), prev(it);
            std::string result;
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
                    } else if (is_digit(c)) {
                        result += c;
                    } else {
                        return token();
                    }
                } else {
                    if (is_digit(c)) {
                        result += c;
                    } else if ('.' == c) {
                        if ('-' == result[i-1] || '+' == result[i-1]) {
                            return token();
                        } else if (found_dot) {
                            unget_char(c);
                            break;
                        } else {
                            found_dot = true;
                            result += c;
                        }
                    } else {
                        if ('.' == result[i-1]) {
                            return token();
                        } else if ('-' == result[i-1] || '+' == result[i-1]) {
                            return token();
                        } else {
                            ++i;
                            unget_char(c);
                            break;
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
