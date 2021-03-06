#ifndef BOOSTER_TEXT_TEMPLATE_SYMBOL_TYPE_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_SYMBOL_TYPE_HPP_INCLUDED

#include <string>

namespace booster {
    namespace text_template {
        
        enum symbol_type {
            ts_unknown = -1,
            ts_eof,
            ts_text,
            ts_quick_print,
            ts_double,
            ts_comma,
            ts_semicolon,
            nts_template,
            nts_quick_print,
            nts_quick_print_tail
        };
        
        inline std::string symbol_type_description(symbol_type t) {
            switch (t) {
                case ts_unknown:        return "ts_unknown";
                case ts_eof:            return "ts_eof";
                case ts_text:           return "ts_text";
                case ts_double:         return "ts_double";
                case ts_semicolon:      return "ts_semicolon";
                case nts_template:      return "nts_template";
                case ts_quick_print:    return "ts_quick_print";
                case nts_quick_print:   return "nts_quick_print";
                default:
                    return "UNKNOWN SYMBOL TYPE";
            }
        }
        
    }
}


#endif // #ifndef BOOSTER_TEXT_TEMPLATE_SYMBOL_TYPE_HPP_INCLUDED
