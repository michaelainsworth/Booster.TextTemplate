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
            nts_template
        };
        
        inline std::string symbol_type_description(symbol_type t) {
            switch (t) {
                case ts_unknown:        return "ts_unknown";
                case ts_eof:            return "ts_eof";
                case ts_text:           return "ts_text";
                case nts_template:      return "nts_template";
                case ts_quick_print:    return "ts_quick_print";
                default:
                    return "UNKNOWN SYMBOL TYPE";
            }
        }
        
    }
}


#endif // #ifndef BOOSTER_TEXT_TEMPLATE_SYMBOL_TYPE_HPP_INCLUDED
