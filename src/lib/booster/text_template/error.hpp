#ifndef BOOSTER_TEXT_TEMPLATE_ERROR_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_ERROR_HPP_INCLUDED

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>

namespace booster {
    namespace error {

        // =====================================================================
        // Enumeration: errc
        // =====================================================================
        
        /*!
         \brief The errc enumeration contains the basic error code values used
         by the text template library.
         */
        enum errc {
            ok = 0
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
                    default:
                        return "Unknown error.";
                }
            }
            
        };
        
    }
}

#endif // #ifndef BOOSTER_TEXT_TEMPLATE_ERROR_HPP_INCLUDED
