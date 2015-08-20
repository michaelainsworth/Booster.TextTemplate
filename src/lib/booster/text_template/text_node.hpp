#ifndef BOOSTER_TEXT_TEMPLATE_TEXT_NODE_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_TEXT_NODE_HPP_INCLUDED

#include <booster/text_template/node.hpp>
#include <string>

namespace booster {
    namespace text_template {
        
        // =====================================================================
        // Interface
        // =====================================================================
        
        /*!
         \brief The text node class represents unparsed text.
         */
        class text_node : public node {
        public:
            
            // -----------------------------------------------------------------
            // Friends
            // -----------------------------------------------------------------
            
            template<typename I> friend class parser;
            
            // -----------------------------------------------------------------
            // Typedefs
            // -----------------------------------------------------------------
            
            typedef std::string string_type;
        
            // -----------------------------------------------------------------
            // Lifecycle
            // -----------------------------------------------------------------
            
            text_node(const string_type& text);
            virtual ~text_node() {}
            
            // -----------------------------------------------------------------
            // Execution
            // -----------------------------------------------------------------
            
            virtual void execute(std::ostream& os);
            
            // -----------------------------------------------------------------
            // Variables
            // -----------------------------------------------------------------
            
        private:
            
            string_type text_;
            
        };
        
        // =====================================================================
        // Implementation
        // =====================================================================
        
        inline text_node::text_node(const string_type& text) : text_(text) {}
        
        inline void text_node::execute(std::ostream& os) {
            os.write(text_.c_str(), text_.size());
        }
        
    }
}

#endif // #ifndef BOOSTER_TEXT_TEMPLATE_TEXT_NODE_HPP_INCLUDED
