#ifndef BOOSTER_TEXT_TEMPLATE_COMMON_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_COMMON_HPP_INCLUDED

#ifdef NDEBUG
#define TTDEBUG(...)
#else
#define TTDEBUG(...) do { std::cerr << __VA_ARGS__ << std::endl; } while (0);
#endif // #ifdef NDEBUG

namespace booster {
    namespace text_template {
        
        //! \todo The double_type and integer_type typedefs may be need to use
        //! macros or a standard c++ traits file (climits?)
        typedef long double double_type;
        typedef long long int integer_type;
        
        typedef unsigned line_type;
        typedef unsigned column_type;
        typedef unsigned offset_type;
        
    }
}

#endif // #ifndef BOOSTER_TEXT_TEMPLATE_COMMON_HPP_INCLUDED
