#ifndef BOOSTER_TEXT_TEMPLATE_TOKEN_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_TOKEN_HPP_INCLUDED

#include <booster/text_template/common.hpp>
#include <booster/text_template/symbol_type.hpp>
#include <iostream>
#include <string>

namespace booster {
    namespace text_template {
        
        // =====================================================================
        // Interface
        // =====================================================================
        
        /*!
         \brief The token class represents a recognised terminal symbol from
         the series of input characters.
         
         The token class is a regular (public access) struct, because it is
         simply a data-container with no behaviour associated with it.
         */
        struct token {
            
            // -----------------------------------------------------------------
            // Typedefs
            // -----------------------------------------------------------------
            
            typedef std::string string_type;
            
            // -----------------------------------------------------------------
            // Lifecycle
            // -----------------------------------------------------------------
            
            /*!
             \brief The default constructor sets the symbol type to ts_unknown,
             the line, column and offset to 0 and the value to an empty string.
             
             The default constructor is usually used when a token is created
             and immediately overridden with, e.g., a value from the lexer.
             */
            token() : filename("unknown"), line(0), column(0), offset(0),
            type(ts_unknown), value("") {}
            
            /*!
             \brief This constructor is used to create the token with explicit
             values.
             */
            token(const string_type& f, line_type l, column_type c,
                  offset_type o, symbol_type s, const string_type& v)
            : filename(f), line(l), column(c), offset(o), type(s), value(v) {}
            

            // -----------------------------------------------------------------
            // Operators
            // -----------------------------------------------------------------
            
            /*!
             \brief The operator bool can be used to detect whether or not
             a token is retrieved from the lexer.
             */
            operator bool() {
                return type != ts_unknown;
            }
            
            // -----------------------------------------------------------------
            // Variables
            // -----------------------------------------------------------------
            
            string_type filename;
            line_type line;
            column_type column;
            offset_type offset;
            symbol_type type;
            string_type value;
            
        };
        
        std::ostream& operator <<(std::ostream& os, const token& tk) {
            os << tk.filename << ":" << tk.line << ":" << tk.column
               << " => " << symbol_type_description(tk.type) << " (" << tk.type
               << ") => ";
            
            os << "\"";
            for (token::string_type::size_type i = 0, s = tk.value.length();
                 i < s; ++i) {
                char c = tk.value[i];
                
                if ('\r' == c) {
                    os << "\\r";
                } else if ('\n' == c) {
                    os << "\\n";
                } else if ('\t' == c) {
                    os << "\\t";
                } else if ('"' == c) {
                    os << "\\\"";
                } else {
                    os << c;
                }
                
                if (i > 15) {
                    os << "\"...";
                    break;
                }
                
                if (i == s - 1) {
                    os << "\"";
                }
            }
            
            return os;
        }
        
    }
}

#endif // #ifndef BOOSTER_TEXT_TEMPLATE_TOKEN_HPP_INCLUDED
