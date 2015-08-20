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
    {
        text_template tpl;
        string in("<b>Hello</b>");
        BOOST_CHECK_NO_THROW(tpl = parse(in.begin(), in.end()));
        BOOST_CHECK(in == tpl());
    }
    
    {
        text_template tpl;
        string in("<b>Hello @print</b>");
        BOOST_CHECK_NO_THROW(tpl = parse(in.begin(), in.end()));
        BOOST_CHECK(in == tpl());
    }
}