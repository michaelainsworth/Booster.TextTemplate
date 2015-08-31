#ifndef BOOSTER_TEXT_TEMPLATE_TEXT_TEMPLATE_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_TEXT_TEMPLATE_HPP_INCLUDED

#include <boost/shared_ptr.hpp>
#include <booster/text_template/parent_node.hpp>

namespace booster {
    namespace text_template {
        
        // =====================================================================
        // Interface
        // =====================================================================
        
        /*!
         \brief Represents a textual template.
         
         The text template can be "inflated" with a dictionary of named data
         using the operator () function.
         */
        class text_template {
        public:
            
            // -----------------------------------------------------------------
            // Friends
            // -----------------------------------------------------------------
            
            template<typename I> friend class parser;
            
            // -----------------------------------------------------------------
            // Typedefs
            // -----------------------------------------------------------------
            
            typedef boost::shared_ptr<parent_node> parent_node_ptr;
            
            // -----------------------------------------------------------------
            // Lifecycle
            // -----------------------------------------------------------------
            
            /*!
             \brief The default constructor allocates a new parent_node in
             which to store the top-level parse elements.
             */
            text_template();
            
            /*!
             \brief The destructor does nothing.
             */
            virtual ~text_template() {}
            
            // -----------------------------------------------------------------
            // Execution
            // -----------------------------------------------------------------
            
            std::string operator()();
            std::string operator()(boost::system::error_condition& e);
            
            // -----------------------------------------------------------------
            // Variables
            // -----------------------------------------------------------------
            
        private:
            
            parent_node_ptr nodes_;
            
        };
        
        // =====================================================================
        // Implementation
        // =====================================================================
        
        inline text_template::text_template() : nodes_() {}
        
        inline std::string text_template::operator()() {
            boost::system::error_condition e;
            std::string result = (*this)(e);
            if (e) {
                throw boost::system::system_error(e.value(), e.category());
            }
            return result;
        }
        
        inline std::string text_template::operator()(boost::system::error_condition& e) {
            if (!nodes_) {
                e = boost::system::error_condition(template_uninitialised, get_error_category());
                return "";
            }
            
            std::ostringstream os;
            nodes_->execute(os);
            return os.str();
        }
        
    }
}

#endif // #ifndef BOOSTER_TEXT_TEMPLATE_TEXT_TEMPLATE_HPP_INCLUDED
