#ifndef BOOSTER_TEXT_TEMPLATE_DICTIONARY_HPP_INCLUDED
#define BOOSTER_TEXT_TEMPLATE_DICTIONARY_HPP_INCLUDED

#include <booster/text_template/value.hpp>
#include <map>
#include <boost/shared_ptr.hpp>
#include <string>

namespace booster {
    namespace text_template {
        
        // =====================================================================
        // Interface
        // =====================================================================
        
        /*!
         \brief A dictionary represents named data supplied to a text
         template.
         */
        class dictionary {
        public:
            
            // -----------------------------------------------------------------
            // Typedefs
            // -----------------------------------------------------------------
            
            typedef std::string string_type;
            typedef std::map<std::string,value*> map_type;
            typedef boost::shared_ptr<map_type> map_ptr_type;
            
            // -----------------------------------------------------------------
            // Lifecycle
            // -----------------------------------------------------------------
            
            /*!
             \brief The destructor checks to see if it holds the last
             reference to the internal map, and if so, deletes the values.
             */
            virtual ~dictionary();
            
            // -----------------------------------------------------------------
            // Mutators
            // -----------------------------------------------------------------

            /*!
             \brief The operator () function defines a new, named
             atomic_value.
             
             If the name specified is already in use, this method will first
             delete the previous atomic_value.
             
             The value of *this is returned to allow method chaining.
             */
            template<typename T>
            dictionary& operator ()(const string_type& name, const T& data);
            
            // -----------------------------------------------------------------
            // Variables
            // -----------------------------------------------------------------
            
        private:
            
            /*!
             \brief The values_ member contains the named data.
             */
            map_ptr_type values_;
            
        };
        
        // =====================================================================
        // Implementation
        // =====================================================================
        
        template<typename T>
        inline dictionary& dictionary::operator ()(const string_type& name,
                                            const T& data) {
            map_type::iterator it = values_->find(name), end = values_->end();
            
            if (it != end) {
                values_->erase(it);
            }
            
            values_->insert(std::make_pair(name, new atomic_value<T>(data)));
            return *this;
        }
        
    }
}

#endif // #ifndef BOOSTER_TEXT_TEMPLATE_DICTIONARY_HPP_INCLUDED
