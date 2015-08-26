#ifndef BOOSTER_TEXT_TEMPLATE_INPUT_POSITION_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_INPUT_POSITION_HPP_INCLUDED

#include <booster/text_template/common.hpp>
#include <string>

namespace booster {
    namespace text_template {
        
        //! \todo Perhaps this class should be renamed to just position.
        struct input_position {
            
            input_position()
            : filename("unknown"), line(0), column(0), offset(0) {}
            
            input_position(const std::string& f, line_type l, column_type c,
                           offset_type o)
            : filename(f), line(l), column(c), offset(o) {}
            
            std::string filename;
            line_type line;
            column_type column;
            offset_type offset;
            
        };
        
    }
}

#endif // #ifndef BOOSTER_TEXT_TEMPLATE_INPUT_POSITION_HPP_INCLUDED
