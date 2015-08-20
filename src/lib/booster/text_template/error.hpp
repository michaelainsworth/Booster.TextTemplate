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
            value_not_printable
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
                    default:
                        return "Unknown error.";
                }
            }
            
        };
        
        inline error_category& get_error_category() {
            static error_category ec;
            return ec;
        }
        
    }
}

#endif // #ifndef BOOSTER_TEXT_TEMPLATE_ERROR_HPP_INCLUDED
