#ifndef BOOSTER_TEXT_TEMPLATE_VALUE_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_VALUE_HPP_INCLUDED

#include <iostream>

namespace booster {
    namespace text_template {
        
        // =====================================================================
        // Interface
        // =====================================================================
        
        /*!
         \brief A value represents data passed to a text template.
         
         The value class is an abstract base class.
         
         \sa atomic_value
         */
        class value {
        public:
        
            // -----------------------------------------------------------------
            // Lifecycle
            // -----------------------------------------------------------------
            
            /*!
             \brief The destructor does nothing, but is virtual, in order for
             the derived classes to be destroyed appropriately.
             */
            virtual ~value() {}
            
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

#endif // #ifndef BOOSTER_TEXT_TEMPLATE_VALUE_HPP_INCLUDED
