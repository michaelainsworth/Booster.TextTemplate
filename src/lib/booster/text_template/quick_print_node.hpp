#ifndef BOOSTER_TEXT_TEMPLATE_QUICK_PRINT_NODE_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_QUICK_PRINT_NODE_HPP_INCLUDED

#include <booster/text_template/parent_node.hpp>
#include <string>

namespace booster {
    namespace text_template {
        
        // =====================================================================
        // Interface
        // =====================================================================
        
        /*!
         \brief The text node class represents unparsed text.
         */
        class quick_print_node : public parent_node {
        public:
            
            // -----------------------------------------------------------------
            // Friends
            // -----------------------------------------------------------------
            
            template<typename I> friend class parser;
            
            // -----------------------------------------------------------------
            // Lifecycle
            // -----------------------------------------------------------------
            
            virtual ~quick_print_node() {}
            
            // -----------------------------------------------------------------
            // Execution
            // -----------------------------------------------------------------
            
            virtual void execute(std::ostream& os);
            
        };
        
        // =====================================================================
        // Implementation
        // =====================================================================
        
        inline void quick_print_node::execute(std::ostream& os) {
            //! \todo Finish
            os << "QUICK PRINT!";
        }
        
    }
}

#endif // #ifndef BOOSTER_TEXT_TEMPLATE_QUICK_PRINT_NODE_HPP_INCLUDED

