#ifndef BOOSTER_TEXT_TEMPLATE_VALUE_TRAITS_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_VALUE_TRAITS_HPP_INCLUDED

namespace booster {
    namespace text_template {
        
        // =====================================================================
        // Interface: print and no_print
        // =====================================================================
        
        /*!
         \brief The print struct is used to enable the print instruction for
         a template parameter data type.
         */
        struct print {
            static const bool can_print = true;
        };
        
        /*!
         \brief The no_print struct is used to disable the print instruction
         for a template parameter data type.
         */
        struct no_print {
            static const bool can_print = false;
        };
        
        // =====================================================================
        // Interface: value_traits
        // =====================================================================
        
        template<typename T>
        struct value_traits
        : public no_print
        {};
        
        // =====================================================================
        // Interface: value_traits<int>
        // =====================================================================
        
        /// \todo Add specialisations for each built-in type.
        
        template<>
        struct value_traits<int>
        : public print
        {};
        
        
    }
}

#endif // #ifndef BOOSTER_TEXT_TEMPLATE_VALUE_TRAITS_HPP_INCLUDED
