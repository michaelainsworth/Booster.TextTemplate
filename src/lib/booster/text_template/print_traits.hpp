#ifndef BOOSTER_TEXT_TEMPLATE_PRINT_TRAITS_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_PRINT_TRAITS_HPP_INCLUDED

namespace booster {
    namespace text_template {
        
        // =====================================================================
        // Interface: print_traits
        // =====================================================================
        
        /*!
         \brief The print_traits template class is used to control how
         template parameter data types are printed.
         
         If you wish to specialise the text template print instruction for
         a particular data type, you must either specialise the print_traits
         template class (recommended), or define an operator << for a
         std::ostream and your data type.
         */
        template<typename T>
        struct print_traits {
            
            // -----------------------------------------------------------------
            // Typedefs
            // -----------------------------------------------------------------
            
            typedef T data_type;
            
            // -----------------------------------------------------------------
            // Methods
            // -----------------------------------------------------------------
            
            /*!
             \brief The print() function simply calls operator << on \p os
             before returning it.
             */
            static std::ostream& print(std::ostream& os,
                                       const data_type& value);
            
        };
        
        // =====================================================================
        // Interface: print_traits
        // =====================================================================
        
        template<typename T>
        inline std::ostream& print_traits<T>::print(std::ostream& os,
                                                    const data_type& value) {
            return (os << value);
        }
        
    }
}

#endif // #ifndef BOOSTER_TEXT_TEMPLATE_PRINT_TRAITS_HPP_INCLUDED
