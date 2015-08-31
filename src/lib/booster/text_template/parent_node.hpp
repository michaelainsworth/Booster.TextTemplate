#ifndef BOOSTER_TEXT_TEMPLATE_PARENT_NODE_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_PARENT_NODE_HPP_INCLUDED

#include <booster/text_template/node.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace booster {
    namespace text_template {
        
        // =====================================================================
        // Interface
        // =====================================================================
        
        /*!
         \brief The parent_node class represents a collection of nodes.
         
         These nodes are at an equal "depth".
         */
        class parent_node : public node {
        public:
            
            // -----------------------------------------------------------------
            // Friends
            // -----------------------------------------------------------------
            
            template<typename I> friend class parser;
            
            // -----------------------------------------------------------------
            // Typedefs
            // -----------------------------------------------------------------
            
            typedef std::vector<node*> node_vector;
            typedef boost::shared_ptr<node_vector> node_vector_ptr;
            
            // -----------------------------------------------------------------
            // Lifecycle
            // -----------------------------------------------------------------
            
            /*!
             The default constructor allocates a new node_vector in which to
             store the child nodes.
             */
            parent_node(const input_position& pos);
        
            /*!
             If on destruction the parent_node holds the last reference to the
             internal list of child nodes, these nodes are deleted.
             */
            virtual ~parent_node();
            
            // -----------------------------------------------------------------
            // Execution
            // -----------------------------------------------------------------
            
            virtual void describe(std::ostream& os, unsigned depth);
            virtual void execute(std::ostream& os);
            
            // -----------------------------------------------------------------
            // Variables
            // -----------------------------------------------------------------
            
        protected:
            
            node_vector_ptr children_;
            
        };
        
        // =====================================================================
        // Implementation
        // =====================================================================
        
        inline parent_node::parent_node(const input_position& pos) : node(pos),
        children_(new node_vector()) {}
        
        inline parent_node::~parent_node() {
            if (1 == children_.use_count()) {
                for (node_vector::iterator it = children_->begin(),
                     end = children_->end(); it != end; ++it) {
                    delete *it;
                }
                
                children_->clear();
            }
        }
        
        inline void parent_node::describe(std::ostream& os, unsigned depth) {
            node::describe_indent(os, depth) << "parent\n";
            
            for (node_vector::iterator it = children_->begin(),
                 end = children_->end(); it != end; ++it) {
                (*it)->describe(os, depth + 1);
            }
        }
        
        inline void parent_node::execute(std::ostream &os) {
            for (node_vector::iterator it = children_->begin(),
                 end = children_->end(); it != end; ++it) {
                (*it)->execute(os);
            }
        }
        
    }
}

#endif // #ifndef BOOSTER_TEXT_TEMPLATE_PARENT_NODE_HPP_INCLUDED
