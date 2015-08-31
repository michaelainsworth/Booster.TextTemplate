#ifndef BOOSTER_TEXT_TEMPLATE_VALUE_NODE_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_VALUE_NODE_HPP_INCLUDED

#include <booster/text_template/node.hpp>
#include <cmath>
#include <string>

namespace booster {
    namespace text_template {
        
        // =====================================================================
        // Interface
        // =====================================================================
        
        /*!
         \brief The value_node class represents a value.
         */
        template<typename T>
        class value_node : public node {
        public:
            
            // -----------------------------------------------------------------
            // Friends
            // -----------------------------------------------------------------
            
            template<typename I> friend class parser;
            
            // -----------------------------------------------------------------
            // Typedefs
            // -----------------------------------------------------------------
            
            typedef T data_type;
            
            // -----------------------------------------------------------------
            // Lifecycle
            // -----------------------------------------------------------------
            
            value_node(const data_type& data);
            virtual ~value_node() {}
            
            // -----------------------------------------------------------------
            // Execution
            // -----------------------------------------------------------------
            
            virtual void describe(std::ostream& os, unsigned depth);
            virtual void execute(std::ostream& os);
            
            // -----------------------------------------------------------------
            // Variables
            // -----------------------------------------------------------------
            
        private:
            
            data_type data_;
            
        };
        
        // =====================================================================
        // Implementation
        // =====================================================================
        
        template<typename T>
        inline value_node<T>::value_node(const data_type& data) : data_(data) {
        }
        
        template<>
        inline void value_node<double_type>::describe(std::ostream& os, unsigned depth) {
            node::describe_indent(os, depth) << "double (";
            if (std::floor(data_) == data_) {
                os << data_ << ".0";
            } else {
                os << data_;
            }
            os << ")\n";
        }
        
        template<typename T>
        inline void value_node<T>::execute(std::ostream& os) {
            //! \todo Should this go through print traits?
            if (std::floor(data_) == data_) {
                os << data_ << ".0";
            } else {
                os << data_;
            }
        }
        
    }
}

#endif // #ifndef BOOSTER_TEXT_TEMPLATE_VALUE_NODE_HPP_INCLUDED
