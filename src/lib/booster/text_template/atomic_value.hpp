#ifndef BOOSTER_TEXT_TEMPLATE_ATOMIC_VALUE_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_ATOMIC_VALUE_HPP_INCLUDED

#include <booster/text_template/value.hpp>
#include <booster/text_template/value_traits.hpp>
#include <booster/text_template/print_traits.hpp>

namespace booster {
    namespace text_template {
        
        // =====================================================================
        // Interface
        // =====================================================================
        
        /*!
         \brief An atomic_value represents a single value that is passed to a
         text template.
         */
        template<typename T>
        class atomic_value : public value {
        public:
            
            // -----------------------------------------------------------------
            // Typedefs
            // -----------------------------------------------------------------
            
            typedef T data_type;
            typedef T* pointer_type;
            typedef T& reference_type;
            
            // -----------------------------------------------------------------
            // Lifecycle
            // -----------------------------------------------------------------
            
            /*!
             * \brief The destructor does nothing.
             */
            virtual ~atomic_value() {}
            
            // -----------------------------------------------------------------
            // Printing
            // -----------------------------------------------------------------
            
            /*!
             \brief The print() method prints the template parameter data type
             to \p os.
             
             The print() method first determines whether the template
             parameter data type can be printed by inspecting value_traits<T>.
             If the type is not printable, an exception is thrown. Otherwise,
             the value is printed using print_traits<T>.
             
             This function returns \p os in order to chain methods on the
             ostream (e.g., using operator <<).
             */
            virtual std::ostream& print(std::ostream& os);
            
            // -----------------------------------------------------------------
            // Variables
            // -----------------------------------------------------------------
            
        private:
            
            /*!
             \brief The data_ member contains the template parameter value,
             and is passed to the template for operation.
             */
            data_type data_;
            
        };
        
        // =====================================================================
        // Implementation
        // =====================================================================
        
        template<typename T>
        inline std::ostream& atomic_value<T>::print(std::ostream& os) {
            if (!value_traits<T>::can_print) {
                // TODO: Throw
            }
            
            
        }
        
    }
}

#endif // #ifndef BOOSTER_TEXT_TEMPLATE_ATOMIC_VALUE_HPP_INCLUDED
