#ifndef BOOSTER_TEXT_TEMPLATE_ERROR_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_ERROR_HPP_INCLUDED

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>

namespace booster {
    namespace text_template {

        // =====================================================================
        // Enumeration: errc
        // =====================================================================
        
        /*!
         \brief The errc enumeration contains the basic error code values used
         by the text template library.
         */
        enum errc {
            ok = 0,
            value_not_printable,
            non_terminal_unexpected,
            terminal_unexpected,
            template_empty
        };
        
        // =====================================================================
        // Interface: error_category
        // =====================================================================

        /*!
         \brief The error_category class represents a unique category of errors
         for text templates.
         */
        class error_category : public boost::system::error_category {
        public:
            
            const char *name() const { return "Booster.TextTemplate"; }
            
            std::string message(int ev) const {
                switch (ev) {
                    case ok:
                        return "No error.";
                    case value_not_printable:
                        return "The value type is not specified.";
                    case non_terminal_unexpected:
                        return "The non-terminal does not contain a parsing rule.";
                    case terminal_unexpected:
                        return "An unexpected token was encountered.";
                    case template_empty:
                        return "The template has not been initialised. This may be because the parsing of the template failed.";
                    default:
                        return "Unknown error.";
                }
            }
            
        };
        
        inline error_category& get_error_category() {
            static error_category ec;
            return ec;
        }
        
        template<typename E>
        inline boost::system::error_condition make_error(E value) {
            return boost::system::error_condition(value, get_error_category());
        }
        
        template<typename E>
        inline boost::system::error_condition throw_error(E value) {
            throw boost::system::system_error(value, get_error_category());
        }
        
    }
}

#endif // #ifndef BOOSTER_TEXT_TEMPLATE_ERROR_HPP_INCLUDED
