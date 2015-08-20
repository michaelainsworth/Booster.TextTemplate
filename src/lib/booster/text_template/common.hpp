#ifndef BOOSTER_TEXT_TEMPLATE_COMMON_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_COMMON_HPP_INCLUDED

#ifdef NDEBUG
#define TTDEBUG(...)
#else
#define TTDEBUG(...) do { std::cerr << __VA_ARGS__ << std::endl; } while (0);
#endif // #ifdef NDEBUG

#endif // #ifndef BOOSTER_TEXT_TEMPLATE_COMMON_HPP_INCLUDED
