#ifndef BOOSTER_TEXT_TEMPLATE_QUICK_PRINT_NODE_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_QUICK_PRINT_NODE_HPP_INCLUDED

#include <booster/text_template/parent_node.hpp>

namespace booster {
    namespace text_template {
        
        // =====================================================================
        // Interface
        // =====================================================================
        
        //!\todo Rename to print_node?
        
        /*!
         \brief The quick_print_node class represents a parse tree element indicating
         a function is being called.
         */
        class quick_print_node : public parent_node {
        public:
            
            // -----------------------------------------------------------------
            // Lifecycle
            // -----------------------------------------------------------------
            
            quick_print_node(const input_position& p) : parent_node(p) {}
            
            virtual ~quick_print_node() {}
            
            // -----------------------------------------------------------------
            // Execution
            // -----------------------------------------------------------------
            
            virtual void describe(std::ostream& os, unsigned depth);
            virtual void execute(std::ostream& os);
            
        };
        
        inline void quick_print_node::describe(std::ostream& os, unsigned depth) {
            describe_indent(os, depth) << "call\n";
            
            parent_node::node_vector::iterator it = children_->begin(),
            end = children_->end();
            
            for (; it != end; ++it) {
                (*it)->describe(os, depth + 1);
            }
        }
        
        void quick_print_node::execute(std::ostream& os) {
            //! \todo Finish this
            parent_node::node_vector::iterator it = children_->begin(),
            end = children_->end();
            
            for (; it != end; ++it) {
                (*it)->execute(os);
            }
        }

        
    }
}

#endif // #ifndef BOOSTER_TEXT_TEMPLATE_QUICK_PRINT_NODE_HPP_INCLUDED
