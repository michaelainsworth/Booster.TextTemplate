#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE print_test
#include <boost/test/unit_test.hpp>
#include <string>
#include <iostream>
#include <booster/text_template/include.hpp>
using boost::system::error_condition;
using booster::text_template::parse;
using booster::text_template::text_template;
using std::string;

BOOST_AUTO_TEST_CASE(text) {
    
    string in("<b>Hello</b>");
    text_template tpl;
    error_condition e;
    string::iterator begin = in.begin();
    BOOST_CHECK(parse(begin, in.end(), tpl, e));
    BOOST_CHECK(in == tpl());
    
    
//    BOOST_CHECK(text_template.parse(in.begin(), in.end()));
    
    
    
}