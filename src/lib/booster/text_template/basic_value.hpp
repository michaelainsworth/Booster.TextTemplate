#ifndef BOOSTER_TEXT_TEMPLATE_BASIC_VALUE_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_BASIC_VALUE_HPP_INCLUDED

#include <iostream>

namespace booster {
    namespace text_template {
        
        // =====================================================================
        // Interface
        // =====================================================================
        
        /*!
         \brief A basic_value represents data passed to a text template.
         
         The basic_value class is an abstract base class.
         
         \sa value
         */
        class basic_value {
        public:
        
            // -----------------------------------------------------------------
            // Lifecycle
            // -----------------------------------------------------------------
            
            /*!
             \brief The destructor does nothing, but is virtual, in order for
             the derived classes to be destroyed appropriately.
             */
            virtual ~basic_value() {}
            
            // -----------------------------------------------------------------
            // Printing
            // -----------------------------------------------------------------
            
            /*!
             \brief The print() method handles the text template print
             instruction.
             */
            virtual std::ostream& print(std::ostream& os) = 0;
            
        };
        
        // =====================================================================
        // Implementation
        // =====================================================================
        
    }
}

#endif // #ifndef BOOSTER_TEXT_TEMPLATE_BASIC_VALUE_HPP_INCLUDED
