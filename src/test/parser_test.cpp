#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE print_test
#include <boost/test/unit_test.hpp>
#include <string>
#include <iostream>
#include <booster/text_template/include.hpp>
using boost::system::error_condition;
using booster::text_template::parent_node;
using booster::text_template::parse;
using booster::text_template::parser;
using booster::text_template::lexer;
using booster::text_template::text_template;
using std::string;

bool test_double(const std::string& in) {
    std::string::const_iterator it = in.begin(), end = in.end();
    booster::text_template::lexer<std::string::const_iterator> ll;
    booster::text_template::token tk = ll.get_token(it, end);
//    TTDEBUG("GOT IT: " << tk);
    return tk.type == booster::text_template::ts_double;
}

bool test_integer(const std::string& in) {
    //! \todo Fix the integer parsing test.
//    std::string::const_iterator begin = in.begin();
//    parser<std::string::const_iterator> pp;
//    return pp.parse_integer_value(begin, in.end(), parent);
    return true;
}

std::string render(const std::string& in) {
    text_template tpl;;
    tpl = parse(in.begin(), in.end());
    std::string s = tpl();
    return tpl();
}

BOOST_AUTO_TEST_CASE(text) {
    BOOST_CHECK_EQUAL("Hello, world!", render("Hello, world!"));
    BOOST_CHECK_EQUAL("Hello, @print!", render("Hello, @print!"));
    BOOST_CHECK_EQUAL("Hello, 1.0!", render("Hello, @print 1.0;!"));
    BOOST_CHECK_EQUAL("Hello, 1.234!", render("Hello, @print 1.234;!"));
    BOOST_CHECK_EQUAL("Hello, -987.654!", render("Hello, @print -987.654 ;!"));
    //! \todo Multiple doubles (comma-separated).
}

BOOST_AUTO_TEST_CASE(lex_double) {
    BOOST_CHECK_EQUAL(false, test_double(""));
    BOOST_CHECK_EQUAL(false, test_double("-"));
    BOOST_CHECK_EQUAL(false, test_double("-+"));
    BOOST_CHECK_EQUAL(false, test_double("-+0"));
    BOOST_CHECK_EQUAL(false, test_double("-+01"));
    BOOST_CHECK_EQUAL(false, test_double("-+01."));
    BOOST_CHECK_EQUAL(false, test_double("-+0."));
    BOOST_CHECK_EQUAL(false, test_double("-+0.1"));
    BOOST_CHECK_EQUAL(false, test_double("-+1"));
    BOOST_CHECK_EQUAL(false, test_double("-+10"));
    BOOST_CHECK_EQUAL(false, test_double("-+10."));
    BOOST_CHECK_EQUAL(false, test_double("-+1."));
    BOOST_CHECK_EQUAL(false, test_double("-+1.0"));
    BOOST_CHECK_EQUAL(false, test_double("-+."));
    BOOST_CHECK_EQUAL(false, test_double("-+.0"));
    BOOST_CHECK_EQUAL(false, test_double("-+.01"));
    BOOST_CHECK_EQUAL(false, test_double("-+.1"));
    BOOST_CHECK_EQUAL(false, test_double("-+.10"));
    BOOST_CHECK_EQUAL(false, test_double("-0"));
    BOOST_CHECK_EQUAL(false, test_double("-0+"));
    BOOST_CHECK_EQUAL(false, test_double("-0+1"));
    BOOST_CHECK_EQUAL(false, test_double("-0+1."));
    BOOST_CHECK_EQUAL(false, test_double("-0+."));
    BOOST_CHECK_EQUAL(false, test_double("-0+.1"));
    BOOST_CHECK_EQUAL(false, test_double("-01"));
    BOOST_CHECK_EQUAL(false, test_double("-01+"));
    BOOST_CHECK_EQUAL(false, test_double("-01+."));
    BOOST_CHECK_EQUAL(false, test_double("-01."));
    BOOST_CHECK_EQUAL(false, test_double("-01.+"));
    BOOST_CHECK_EQUAL(false, test_double("-0."));
    BOOST_CHECK_EQUAL(false, test_double("-0.+"));
    BOOST_CHECK_EQUAL(false, test_double("-0.+1"));
    BOOST_CHECK_EQUAL(true, test_double("-0.1"));
    BOOST_CHECK_EQUAL(true, test_double("-0.1+"));
    BOOST_CHECK_EQUAL(false, test_double("-1"));
    BOOST_CHECK_EQUAL(false, test_double("-1+"));
    BOOST_CHECK_EQUAL(false, test_double("-1+0"));
    BOOST_CHECK_EQUAL(false, test_double("-1+0."));
    BOOST_CHECK_EQUAL(false, test_double("-1+."));
    BOOST_CHECK_EQUAL(false, test_double("-1+.0"));
    BOOST_CHECK_EQUAL(false, test_double("-10"));
    BOOST_CHECK_EQUAL(false, test_double("-10+"));
    BOOST_CHECK_EQUAL(false, test_double("-10+."));
    BOOST_CHECK_EQUAL(false, test_double("-10."));
    BOOST_CHECK_EQUAL(false, test_double("-10.+"));
    BOOST_CHECK_EQUAL(false, test_double("-1."));
    BOOST_CHECK_EQUAL(false, test_double("-1.+"));
    BOOST_CHECK_EQUAL(false, test_double("-1.+0"));
    BOOST_CHECK_EQUAL(true, test_double("-1.0"));
    BOOST_CHECK_EQUAL(true, test_double("-1.0+"));
    BOOST_CHECK_EQUAL(false, test_double("-."));
    BOOST_CHECK_EQUAL(false, test_double("-.+"));
    BOOST_CHECK_EQUAL(false, test_double("-.+0"));
    BOOST_CHECK_EQUAL(false, test_double("-.+01"));
    BOOST_CHECK_EQUAL(false, test_double("-.+1"));
    BOOST_CHECK_EQUAL(false, test_double("-.+10"));
    BOOST_CHECK_EQUAL(false, test_double("-.0"));
    BOOST_CHECK_EQUAL(false, test_double("-.0+"));
    BOOST_CHECK_EQUAL(false, test_double("-.0+1"));
    BOOST_CHECK_EQUAL(false, test_double("-.01"));
    BOOST_CHECK_EQUAL(false, test_double("-.01+"));
    BOOST_CHECK_EQUAL(false, test_double("-.1"));
    BOOST_CHECK_EQUAL(false, test_double("-.1+"));
    BOOST_CHECK_EQUAL(false, test_double("-.1+0"));
    BOOST_CHECK_EQUAL(false, test_double("-.10"));
    BOOST_CHECK_EQUAL(false, test_double("-.10+"));
    BOOST_CHECK_EQUAL(false, test_double("+"));
    BOOST_CHECK_EQUAL(false, test_double("+-"));
    BOOST_CHECK_EQUAL(false, test_double("+-0"));
    BOOST_CHECK_EQUAL(false, test_double("+-01"));
    BOOST_CHECK_EQUAL(false, test_double("+-01."));
    BOOST_CHECK_EQUAL(false, test_double("+-0."));
    BOOST_CHECK_EQUAL(false, test_double("+-0.1"));
    BOOST_CHECK_EQUAL(false, test_double("+-1"));
    BOOST_CHECK_EQUAL(false, test_double("+-10"));
    BOOST_CHECK_EQUAL(false, test_double("+-10."));
    BOOST_CHECK_EQUAL(false, test_double("+-1."));
    BOOST_CHECK_EQUAL(false, test_double("+-1.0"));
    BOOST_CHECK_EQUAL(false, test_double("+-."));
    BOOST_CHECK_EQUAL(false, test_double("+-.0"));
    BOOST_CHECK_EQUAL(false, test_double("+-.01"));
    BOOST_CHECK_EQUAL(false, test_double("+-.1"));
    BOOST_CHECK_EQUAL(false, test_double("+-.10"));
    BOOST_CHECK_EQUAL(false, test_double("+0"));
    BOOST_CHECK_EQUAL(false, test_double("+0-"));
    BOOST_CHECK_EQUAL(false, test_double("+0-1"));
    BOOST_CHECK_EQUAL(false, test_double("+0-1."));
    BOOST_CHECK_EQUAL(false, test_double("+0-."));
    BOOST_CHECK_EQUAL(false, test_double("+0-.1"));
    BOOST_CHECK_EQUAL(false, test_double("+01"));
    BOOST_CHECK_EQUAL(false, test_double("+01-"));
    BOOST_CHECK_EQUAL(false, test_double("+01-."));
    BOOST_CHECK_EQUAL(false, test_double("+01."));
    BOOST_CHECK_EQUAL(false, test_double("+01.-"));
    BOOST_CHECK_EQUAL(false, test_double("+0."));
    BOOST_CHECK_EQUAL(false, test_double("+0.-"));
    BOOST_CHECK_EQUAL(false, test_double("+0.-1"));
    BOOST_CHECK_EQUAL(true, test_double("+0.1"));
    BOOST_CHECK_EQUAL(true, test_double("+0.1-"));
    BOOST_CHECK_EQUAL(false, test_double("+1"));
    BOOST_CHECK_EQUAL(false, test_double("+1-"));
    BOOST_CHECK_EQUAL(false, test_double("+1-0"));
    BOOST_CHECK_EQUAL(false, test_double("+1-0."));
    BOOST_CHECK_EQUAL(false, test_double("+1-."));
    BOOST_CHECK_EQUAL(false, test_double("+1-.0"));
    BOOST_CHECK_EQUAL(false, test_double("+10"));
    BOOST_CHECK_EQUAL(false, test_double("+10-"));
    BOOST_CHECK_EQUAL(false, test_double("+10-."));
    BOOST_CHECK_EQUAL(false, test_double("+10."));
    BOOST_CHECK_EQUAL(false, test_double("+10.-"));
    BOOST_CHECK_EQUAL(false, test_double("+1."));
    BOOST_CHECK_EQUAL(false, test_double("+1.-"));
    BOOST_CHECK_EQUAL(false, test_double("+1.-0"));
    BOOST_CHECK_EQUAL(true, test_double("+1.0"));
    BOOST_CHECK_EQUAL(true, test_double("+1.0-"));
    BOOST_CHECK_EQUAL(false, test_double("+."));
    BOOST_CHECK_EQUAL(false, test_double("+.-"));
    BOOST_CHECK_EQUAL(false, test_double("+.-0"));
    BOOST_CHECK_EQUAL(false, test_double("+.-01"));
    BOOST_CHECK_EQUAL(false, test_double("+.-1"));
    BOOST_CHECK_EQUAL(false, test_double("+.-10"));
    BOOST_CHECK_EQUAL(false, test_double("+.0"));
    BOOST_CHECK_EQUAL(false, test_double("+.0-"));
    BOOST_CHECK_EQUAL(false, test_double("+.0-1"));
    BOOST_CHECK_EQUAL(false, test_double("+.01"));
    BOOST_CHECK_EQUAL(false, test_double("+.01-"));
    BOOST_CHECK_EQUAL(false, test_double("+.1"));
    BOOST_CHECK_EQUAL(false, test_double("+.1-"));
    BOOST_CHECK_EQUAL(false, test_double("+.1-0"));
    BOOST_CHECK_EQUAL(false, test_double("+.10"));
    BOOST_CHECK_EQUAL(false, test_double("+.10-"));
    BOOST_CHECK_EQUAL(false, test_double("0"));
    BOOST_CHECK_EQUAL(false, test_double("0-"));
    BOOST_CHECK_EQUAL(false, test_double("0-+"));
    BOOST_CHECK_EQUAL(false, test_double("0-+1"));
    BOOST_CHECK_EQUAL(false, test_double("0-+1."));
    BOOST_CHECK_EQUAL(false, test_double("0-+."));
    BOOST_CHECK_EQUAL(false, test_double("0-+.1"));
    BOOST_CHECK_EQUAL(false, test_double("0-1"));
    BOOST_CHECK_EQUAL(false, test_double("0-1+"));
    BOOST_CHECK_EQUAL(false, test_double("0-1+."));
    BOOST_CHECK_EQUAL(false, test_double("0-1."));
    BOOST_CHECK_EQUAL(false, test_double("0-1.+"));
    BOOST_CHECK_EQUAL(false, test_double("0-."));
    BOOST_CHECK_EQUAL(false, test_double("0-.+"));
    BOOST_CHECK_EQUAL(false, test_double("0-.+1"));
    BOOST_CHECK_EQUAL(false, test_double("0-.1"));
    BOOST_CHECK_EQUAL(false, test_double("0-.1+"));
    BOOST_CHECK_EQUAL(false, test_double("0+"));
    BOOST_CHECK_EQUAL(false, test_double("0+-"));
    BOOST_CHECK_EQUAL(false, test_double("0+-1"));
    BOOST_CHECK_EQUAL(false, test_double("0+-1."));
    BOOST_CHECK_EQUAL(false, test_double("0+-."));
    BOOST_CHECK_EQUAL(false, test_double("0+-.1"));
    BOOST_CHECK_EQUAL(false, test_double("0+1"));
    BOOST_CHECK_EQUAL(false, test_double("0+1-"));
    BOOST_CHECK_EQUAL(false, test_double("0+1-."));
    BOOST_CHECK_EQUAL(false, test_double("0+1."));
    BOOST_CHECK_EQUAL(false, test_double("0+1.-"));
    BOOST_CHECK_EQUAL(false, test_double("0+."));
    BOOST_CHECK_EQUAL(false, test_double("0+.-"));
    BOOST_CHECK_EQUAL(false, test_double("0+.-1"));
    BOOST_CHECK_EQUAL(false, test_double("0+.1"));
    BOOST_CHECK_EQUAL(false, test_double("0+.1-"));
    BOOST_CHECK_EQUAL(false, test_double("01"));
    BOOST_CHECK_EQUAL(false, test_double("01-"));
    BOOST_CHECK_EQUAL(false, test_double("01-+"));
    BOOST_CHECK_EQUAL(false, test_double("01-+."));
    BOOST_CHECK_EQUAL(false, test_double("01-."));
    BOOST_CHECK_EQUAL(false, test_double("01-.+"));
    BOOST_CHECK_EQUAL(false, test_double("01+"));
    BOOST_CHECK_EQUAL(false, test_double("01+-"));
    BOOST_CHECK_EQUAL(false, test_double("01+-."));
    BOOST_CHECK_EQUAL(false, test_double("01+."));
    BOOST_CHECK_EQUAL(false, test_double("01+.-"));
    BOOST_CHECK_EQUAL(false, test_double("01."));
    BOOST_CHECK_EQUAL(false, test_double("01.-"));
    BOOST_CHECK_EQUAL(false, test_double("01.-+"));
    BOOST_CHECK_EQUAL(false, test_double("01.+"));
    BOOST_CHECK_EQUAL(false, test_double("01.+-"));
    BOOST_CHECK_EQUAL(false, test_double("0."));
    BOOST_CHECK_EQUAL(false, test_double("0.-"));
    BOOST_CHECK_EQUAL(false, test_double("0.-+"));
    BOOST_CHECK_EQUAL(false, test_double("0.-+1"));
    BOOST_CHECK_EQUAL(false, test_double("0.-1"));
    BOOST_CHECK_EQUAL(false, test_double("0.-1+"));
    BOOST_CHECK_EQUAL(false, test_double("0.+"));
    BOOST_CHECK_EQUAL(false, test_double("0.+-"));
    BOOST_CHECK_EQUAL(false, test_double("0.+-1"));
    BOOST_CHECK_EQUAL(false, test_double("0.+1"));
    BOOST_CHECK_EQUAL(false, test_double("0.+1-"));
    BOOST_CHECK_EQUAL(true, test_double("0.1"));
    BOOST_CHECK_EQUAL(true, test_double("0.1-"));
    BOOST_CHECK_EQUAL(true, test_double("0.1-+"));
    BOOST_CHECK_EQUAL(true, test_double("0.1+"));
    BOOST_CHECK_EQUAL(true, test_double("0.1+-"));
    BOOST_CHECK_EQUAL(false, test_double("1"));
    BOOST_CHECK_EQUAL(false, test_double("1-"));
    BOOST_CHECK_EQUAL(false, test_double("1-+"));
    BOOST_CHECK_EQUAL(false, test_double("1-+0"));
    BOOST_CHECK_EQUAL(false, test_double("1-+0."));
    BOOST_CHECK_EQUAL(false, test_double("1-+."));
    BOOST_CHECK_EQUAL(false, test_double("1-+.0"));
    BOOST_CHECK_EQUAL(false, test_double("1-0"));
    BOOST_CHECK_EQUAL(false, test_double("1-0+"));
    BOOST_CHECK_EQUAL(false, test_double("1-0+."));
    BOOST_CHECK_EQUAL(false, test_double("1-0."));
    BOOST_CHECK_EQUAL(false, test_double("1-0.+"));
    BOOST_CHECK_EQUAL(false, test_double("1-."));
    BOOST_CHECK_EQUAL(false, test_double("1-.+"));
    BOOST_CHECK_EQUAL(false, test_double("1-.+0"));
    BOOST_CHECK_EQUAL(false, test_double("1-.0"));
    BOOST_CHECK_EQUAL(false, test_double("1-.0+"));
    BOOST_CHECK_EQUAL(false, test_double("1+"));
    BOOST_CHECK_EQUAL(false, test_double("1+-"));
    BOOST_CHECK_EQUAL(false, test_double("1+-0"));
    BOOST_CHECK_EQUAL(false, test_double("1+-0."));
    BOOST_CHECK_EQUAL(false, test_double("1+-."));
    BOOST_CHECK_EQUAL(false, test_double("1+-.0"));
    BOOST_CHECK_EQUAL(false, test_double("1+0"));
    BOOST_CHECK_EQUAL(false, test_double("1+0-"));
    BOOST_CHECK_EQUAL(false, test_double("1+0-."));
    BOOST_CHECK_EQUAL(false, test_double("1+0."));
    BOOST_CHECK_EQUAL(false, test_double("1+0.-"));
    BOOST_CHECK_EQUAL(false, test_double("1+."));
    BOOST_CHECK_EQUAL(false, test_double("1+.-"));
    BOOST_CHECK_EQUAL(false, test_double("1+.-0"));
    BOOST_CHECK_EQUAL(false, test_double("1+.0"));
    BOOST_CHECK_EQUAL(false, test_double("1+.0-"));
    BOOST_CHECK_EQUAL(false, test_double("10"));
    BOOST_CHECK_EQUAL(false, test_double("10-"));
    BOOST_CHECK_EQUAL(false, test_double("10-+"));
    BOOST_CHECK_EQUAL(false, test_double("10-+."));
    BOOST_CHECK_EQUAL(false, test_double("10-."));
    BOOST_CHECK_EQUAL(false, test_double("10-.+"));
    BOOST_CHECK_EQUAL(false, test_double("10+"));
    BOOST_CHECK_EQUAL(false, test_double("10+-"));
    BOOST_CHECK_EQUAL(false, test_double("10+-."));
    BOOST_CHECK_EQUAL(false, test_double("10+."));
    BOOST_CHECK_EQUAL(false, test_double("10+.-"));
    BOOST_CHECK_EQUAL(false, test_double("10."));
    BOOST_CHECK_EQUAL(false, test_double("10.-"));
    BOOST_CHECK_EQUAL(false, test_double("10.-+"));
    BOOST_CHECK_EQUAL(false, test_double("10.+"));
    BOOST_CHECK_EQUAL(false, test_double("10.+-"));
    BOOST_CHECK_EQUAL(false, test_double("1."));
    BOOST_CHECK_EQUAL(false, test_double("1.-"));
    BOOST_CHECK_EQUAL(false, test_double("1.-+"));
    BOOST_CHECK_EQUAL(false, test_double("1.-+0"));
    BOOST_CHECK_EQUAL(false, test_double("1.-0"));
    BOOST_CHECK_EQUAL(false, test_double("1.-0+"));
    BOOST_CHECK_EQUAL(false, test_double("1.+"));
    BOOST_CHECK_EQUAL(false, test_double("1.+-"));
    BOOST_CHECK_EQUAL(false, test_double("1.+-0"));
    BOOST_CHECK_EQUAL(false, test_double("1.+0"));
    BOOST_CHECK_EQUAL(false, test_double("1.+0-"));
    BOOST_CHECK_EQUAL(true, test_double("1.0"));
    BOOST_CHECK_EQUAL(true, test_double("1.0-"));
    BOOST_CHECK_EQUAL(true, test_double("1.0-+"));
    BOOST_CHECK_EQUAL(true, test_double("1.0+"));
    BOOST_CHECK_EQUAL(true, test_double("1.0+-"));
    BOOST_CHECK_EQUAL(false, test_double("."));
    BOOST_CHECK_EQUAL(false, test_double(".-"));
    BOOST_CHECK_EQUAL(false, test_double(".-+"));
    BOOST_CHECK_EQUAL(false, test_double(".-+0"));
    BOOST_CHECK_EQUAL(false, test_double(".-+01"));
    BOOST_CHECK_EQUAL(false, test_double(".-+1"));
    BOOST_CHECK_EQUAL(false, test_double(".-+10"));
    BOOST_CHECK_EQUAL(false, test_double(".-0"));
    BOOST_CHECK_EQUAL(false, test_double(".-0+"));
    BOOST_CHECK_EQUAL(false, test_double(".-0+1"));
    BOOST_CHECK_EQUAL(false, test_double(".-01"));
    BOOST_CHECK_EQUAL(false, test_double(".-01+"));
    BOOST_CHECK_EQUAL(false, test_double(".-1"));
    BOOST_CHECK_EQUAL(false, test_double(".-1+"));
    BOOST_CHECK_EQUAL(false, test_double(".-1+0"));
    BOOST_CHECK_EQUAL(false, test_double(".-10"));
    BOOST_CHECK_EQUAL(false, test_double(".-10+"));
    BOOST_CHECK_EQUAL(false, test_double(".+"));
    BOOST_CHECK_EQUAL(false, test_double(".+-"));
    BOOST_CHECK_EQUAL(false, test_double(".+-0"));
    BOOST_CHECK_EQUAL(false, test_double(".+-01"));
    BOOST_CHECK_EQUAL(false, test_double(".+-1"));
    BOOST_CHECK_EQUAL(false, test_double(".+-10"));
    BOOST_CHECK_EQUAL(false, test_double(".+0"));
    BOOST_CHECK_EQUAL(false, test_double(".+0-"));
    BOOST_CHECK_EQUAL(false, test_double(".+0-1"));
    BOOST_CHECK_EQUAL(false, test_double(".+01"));
    BOOST_CHECK_EQUAL(false, test_double(".+01-"));
    BOOST_CHECK_EQUAL(false, test_double(".+1"));
    BOOST_CHECK_EQUAL(false, test_double(".+1-"));
    BOOST_CHECK_EQUAL(false, test_double(".+1-0"));
    BOOST_CHECK_EQUAL(false, test_double(".+10"));
    BOOST_CHECK_EQUAL(false, test_double(".+10-"));
    BOOST_CHECK_EQUAL(false, test_double(".0"));
    BOOST_CHECK_EQUAL(false, test_double(".0-"));
    BOOST_CHECK_EQUAL(false, test_double(".0-+"));
    BOOST_CHECK_EQUAL(false, test_double(".0-+1"));
    BOOST_CHECK_EQUAL(false, test_double(".0-1"));
    BOOST_CHECK_EQUAL(false, test_double(".0-1+"));
    BOOST_CHECK_EQUAL(false, test_double(".0+"));
    BOOST_CHECK_EQUAL(false, test_double(".0+-"));
    BOOST_CHECK_EQUAL(false, test_double(".0+-1"));
    BOOST_CHECK_EQUAL(false, test_double(".0+1"));
    BOOST_CHECK_EQUAL(false, test_double(".0+1-"));
    BOOST_CHECK_EQUAL(false, test_double(".01"));
    BOOST_CHECK_EQUAL(false, test_double(".01-"));
    BOOST_CHECK_EQUAL(false, test_double(".01-+"));
    BOOST_CHECK_EQUAL(false, test_double(".01+"));
    BOOST_CHECK_EQUAL(false, test_double(".01+-"));
    BOOST_CHECK_EQUAL(false, test_double(".1"));
    BOOST_CHECK_EQUAL(false, test_double(".1-"));
    BOOST_CHECK_EQUAL(false, test_double(".1-+"));
    BOOST_CHECK_EQUAL(false, test_double(".1-+0"));
    BOOST_CHECK_EQUAL(false, test_double(".1-0"));
    BOOST_CHECK_EQUAL(false, test_double(".1-0+"));
    BOOST_CHECK_EQUAL(false, test_double(".1+"));
    BOOST_CHECK_EQUAL(false, test_double(".1+-"));
    BOOST_CHECK_EQUAL(false, test_double(".1+-0"));
    BOOST_CHECK_EQUAL(false, test_double(".1+0"));
    BOOST_CHECK_EQUAL(false, test_double(".1+0-"));
    BOOST_CHECK_EQUAL(false, test_double(".10"));
    BOOST_CHECK_EQUAL(false, test_double(".10-"));
    BOOST_CHECK_EQUAL(false, test_double(".10-+"));
    BOOST_CHECK_EQUAL(false, test_double(".10+"));
    BOOST_CHECK_EQUAL(false, test_double(".10+-"));
    BOOST_CHECK_EQUAL(true, test_double("-987654321.123456789876543212345678987654321"));
}

BOOST_AUTO_TEST_CASE(parse_integer_value) {
    //! \todo Re-enable.
//    BOOST_CHECK_EQUAL(false, test_integer(""));
//    BOOST_CHECK_EQUAL(false, test_integer("-"));
//    BOOST_CHECK_EQUAL(false, test_integer("-+"));
//    BOOST_CHECK_EQUAL(false, test_integer("-+0"));
//    BOOST_CHECK_EQUAL(false, test_integer("-+01"));
//    BOOST_CHECK_EQUAL(false, test_integer("-+01a"));
//    BOOST_CHECK_EQUAL(false, test_integer("-+0a"));
//    BOOST_CHECK_EQUAL(false, test_integer("-+0a1"));
//    BOOST_CHECK_EQUAL(false, test_integer("-+1"));
//    BOOST_CHECK_EQUAL(false, test_integer("-+10"));
//    BOOST_CHECK_EQUAL(false, test_integer("-+10a"));
//    BOOST_CHECK_EQUAL(false, test_integer("-+1a"));
//    BOOST_CHECK_EQUAL(false, test_integer("-+1a0"));
//    BOOST_CHECK_EQUAL(false, test_integer("-+a"));
//    BOOST_CHECK_EQUAL(false, test_integer("-+a0"));
//    BOOST_CHECK_EQUAL(false, test_integer("-+a01"));
//    BOOST_CHECK_EQUAL(false, test_integer("-+a1"));
//    BOOST_CHECK_EQUAL(false, test_integer("-+a10"));
//    BOOST_CHECK_EQUAL(true, test_integer("-0"));
//    BOOST_CHECK_EQUAL(true, test_integer("-0+"));
//    BOOST_CHECK_EQUAL(true, test_integer("-0+1"));
//    BOOST_CHECK_EQUAL(true, test_integer("-0+1a"));
//    BOOST_CHECK_EQUAL(true, test_integer("-0+a"));
//    BOOST_CHECK_EQUAL(true, test_integer("-0+a1"));
//    BOOST_CHECK_EQUAL(true, test_integer("-01"));
//    BOOST_CHECK_EQUAL(true, test_integer("-01+"));
//    BOOST_CHECK_EQUAL(true, test_integer("-01+a"));
//    BOOST_CHECK_EQUAL(true, test_integer("-01a"));
//    BOOST_CHECK_EQUAL(true, test_integer("-01a+"));
//    BOOST_CHECK_EQUAL(true, test_integer("-0a"));
//    BOOST_CHECK_EQUAL(true, test_integer("-0a+"));
//    BOOST_CHECK_EQUAL(true, test_integer("-0a+1"));
//    BOOST_CHECK_EQUAL(true, test_integer("-0a1"));
//    BOOST_CHECK_EQUAL(true, test_integer("-0a1+"));
//    BOOST_CHECK_EQUAL(true, test_integer("-1"));
//    BOOST_CHECK_EQUAL(true, test_integer("-1+"));
//    BOOST_CHECK_EQUAL(true, test_integer("-1+0"));
//    BOOST_CHECK_EQUAL(true, test_integer("-1+0a"));
//    BOOST_CHECK_EQUAL(true, test_integer("-1+a"));
//    BOOST_CHECK_EQUAL(true, test_integer("-1+a0"));
//    BOOST_CHECK_EQUAL(true, test_integer("-10"));
//    BOOST_CHECK_EQUAL(true, test_integer("-10+"));
//    BOOST_CHECK_EQUAL(true, test_integer("-10+a"));
//    BOOST_CHECK_EQUAL(true, test_integer("-10a"));
//    BOOST_CHECK_EQUAL(true, test_integer("-10a+"));
//    BOOST_CHECK_EQUAL(true, test_integer("-1a"));
//    BOOST_CHECK_EQUAL(true, test_integer("-1a+"));
//    BOOST_CHECK_EQUAL(true, test_integer("-1a+0"));
//    BOOST_CHECK_EQUAL(true, test_integer("-1a0"));
//    BOOST_CHECK_EQUAL(true, test_integer("-1a0+"));
//    BOOST_CHECK_EQUAL(false, test_integer("-a"));
//    BOOST_CHECK_EQUAL(false, test_integer("-a+"));
//    BOOST_CHECK_EQUAL(false, test_integer("-a+0"));
//    BOOST_CHECK_EQUAL(false, test_integer("-a+01"));
//    BOOST_CHECK_EQUAL(false, test_integer("-a+1"));
//    BOOST_CHECK_EQUAL(false, test_integer("-a+10"));
//    BOOST_CHECK_EQUAL(false, test_integer("-a0"));
//    BOOST_CHECK_EQUAL(false, test_integer("-a0+"));
//    BOOST_CHECK_EQUAL(false, test_integer("-a0+1"));
//    BOOST_CHECK_EQUAL(false, test_integer("-a01"));
//    BOOST_CHECK_EQUAL(false, test_integer("-a01+"));
//    BOOST_CHECK_EQUAL(false, test_integer("-a1"));
//    BOOST_CHECK_EQUAL(false, test_integer("-a1+"));
//    BOOST_CHECK_EQUAL(false, test_integer("-a1+0"));
//    BOOST_CHECK_EQUAL(false, test_integer("-a10"));
//    BOOST_CHECK_EQUAL(false, test_integer("-a10+"));
//    BOOST_CHECK_EQUAL(false, test_integer("+"));
//    BOOST_CHECK_EQUAL(false, test_integer("+-"));
//    BOOST_CHECK_EQUAL(false, test_integer("+-0"));
//    BOOST_CHECK_EQUAL(false, test_integer("+-01"));
//    BOOST_CHECK_EQUAL(false, test_integer("+-01a"));
//    BOOST_CHECK_EQUAL(false, test_integer("+-0a"));
//    BOOST_CHECK_EQUAL(false, test_integer("+-0a1"));
//    BOOST_CHECK_EQUAL(false, test_integer("+-1"));
//    BOOST_CHECK_EQUAL(false, test_integer("+-10"));
//    BOOST_CHECK_EQUAL(false, test_integer("+-10a"));
//    BOOST_CHECK_EQUAL(false, test_integer("+-1a"));
//    BOOST_CHECK_EQUAL(false, test_integer("+-1a0"));
//    BOOST_CHECK_EQUAL(false, test_integer("+-a"));
//    BOOST_CHECK_EQUAL(false, test_integer("+-a0"));
//    BOOST_CHECK_EQUAL(false, test_integer("+-a01"));
//    BOOST_CHECK_EQUAL(false, test_integer("+-a1"));
//    BOOST_CHECK_EQUAL(false, test_integer("+-a10"));
//    BOOST_CHECK_EQUAL(true, test_integer("+0"));
//    BOOST_CHECK_EQUAL(true, test_integer("+0-"));
//    BOOST_CHECK_EQUAL(true, test_integer("+0-1"));
//    BOOST_CHECK_EQUAL(true, test_integer("+0-1a"));
//    BOOST_CHECK_EQUAL(true, test_integer("+0-a"));
//    BOOST_CHECK_EQUAL(true, test_integer("+0-a1"));
//    BOOST_CHECK_EQUAL(true, test_integer("+01"));
//    BOOST_CHECK_EQUAL(true, test_integer("+01-"));
//    BOOST_CHECK_EQUAL(true, test_integer("+01-a"));
//    BOOST_CHECK_EQUAL(true, test_integer("+01a"));
//    BOOST_CHECK_EQUAL(true, test_integer("+01a-"));
//    BOOST_CHECK_EQUAL(true, test_integer("+0a"));
//    BOOST_CHECK_EQUAL(true, test_integer("+0a-"));
//    BOOST_CHECK_EQUAL(true, test_integer("+0a-1"));
//    BOOST_CHECK_EQUAL(true, test_integer("+0a1"));
//    BOOST_CHECK_EQUAL(true, test_integer("+0a1-"));
//    BOOST_CHECK_EQUAL(true, test_integer("+1"));
//    BOOST_CHECK_EQUAL(true, test_integer("+1-"));
//    BOOST_CHECK_EQUAL(true, test_integer("+1-0"));
//    BOOST_CHECK_EQUAL(true, test_integer("+1-0a"));
//    BOOST_CHECK_EQUAL(true, test_integer("+1-a"));
//    BOOST_CHECK_EQUAL(true, test_integer("+1-a0"));
//    BOOST_CHECK_EQUAL(true, test_integer("+10"));
//    BOOST_CHECK_EQUAL(true, test_integer("+10-"));
//    BOOST_CHECK_EQUAL(true, test_integer("+10-a"));
//    BOOST_CHECK_EQUAL(true, test_integer("+10a"));
//    BOOST_CHECK_EQUAL(true, test_integer("+10a-"));
//    BOOST_CHECK_EQUAL(true, test_integer("+1a"));
//    BOOST_CHECK_EQUAL(true, test_integer("+1a-"));
//    BOOST_CHECK_EQUAL(true, test_integer("+1a-0"));
//    BOOST_CHECK_EQUAL(true, test_integer("+1a0"));
//    BOOST_CHECK_EQUAL(true, test_integer("+1a0-"));
//    BOOST_CHECK_EQUAL(false, test_integer("+a"));
//    BOOST_CHECK_EQUAL(false, test_integer("+a-"));
//    BOOST_CHECK_EQUAL(false, test_integer("+a-0"));
//    BOOST_CHECK_EQUAL(false, test_integer("+a-01"));
//    BOOST_CHECK_EQUAL(false, test_integer("+a-1"));
//    BOOST_CHECK_EQUAL(false, test_integer("+a-10"));
//    BOOST_CHECK_EQUAL(false, test_integer("+a0"));
//    BOOST_CHECK_EQUAL(false, test_integer("+a0-"));
//    BOOST_CHECK_EQUAL(false, test_integer("+a0-1"));
//    BOOST_CHECK_EQUAL(false, test_integer("+a01"));
//    BOOST_CHECK_EQUAL(false, test_integer("+a01-"));
//    BOOST_CHECK_EQUAL(false, test_integer("+a1"));
//    BOOST_CHECK_EQUAL(false, test_integer("+a1-"));
//    BOOST_CHECK_EQUAL(false, test_integer("+a1-0"));
//    BOOST_CHECK_EQUAL(false, test_integer("+a10"));
//    BOOST_CHECK_EQUAL(false, test_integer("+a10-"));
//    BOOST_CHECK_EQUAL(true, test_integer("0"));
//    BOOST_CHECK_EQUAL(true, test_integer("0-"));
//    BOOST_CHECK_EQUAL(true, test_integer("0-+"));
//    BOOST_CHECK_EQUAL(true, test_integer("0-+1"));
//    BOOST_CHECK_EQUAL(true, test_integer("0-+1a"));
//    BOOST_CHECK_EQUAL(true, test_integer("0-+a"));
//    BOOST_CHECK_EQUAL(true, test_integer("0-+a1"));
//    BOOST_CHECK_EQUAL(true, test_integer("0-1"));
//    BOOST_CHECK_EQUAL(true, test_integer("0-1+"));
//    BOOST_CHECK_EQUAL(true, test_integer("0-1+a"));
//    BOOST_CHECK_EQUAL(true, test_integer("0-1a"));
//    BOOST_CHECK_EQUAL(true, test_integer("0-1a+"));
//    BOOST_CHECK_EQUAL(true, test_integer("0-a"));
//    BOOST_CHECK_EQUAL(true, test_integer("0-a+"));
//    BOOST_CHECK_EQUAL(true, test_integer("0-a+1"));
//    BOOST_CHECK_EQUAL(true, test_integer("0-a1"));
//    BOOST_CHECK_EQUAL(true, test_integer("0-a1+"));
//    BOOST_CHECK_EQUAL(true, test_integer("0+"));
//    BOOST_CHECK_EQUAL(true, test_integer("0+-"));
//    BOOST_CHECK_EQUAL(true, test_integer("0+-1"));
//    BOOST_CHECK_EQUAL(true, test_integer("0+-1a"));
//    BOOST_CHECK_EQUAL(true, test_integer("0+-a"));
//    BOOST_CHECK_EQUAL(true, test_integer("0+-a1"));
//    BOOST_CHECK_EQUAL(true, test_integer("0+1"));
//    BOOST_CHECK_EQUAL(true, test_integer("0+1-"));
//    BOOST_CHECK_EQUAL(true, test_integer("0+1-a"));
//    BOOST_CHECK_EQUAL(true, test_integer("0+1a"));
//    BOOST_CHECK_EQUAL(true, test_integer("0+1a-"));
//    BOOST_CHECK_EQUAL(true, test_integer("0+a"));
//    BOOST_CHECK_EQUAL(true, test_integer("0+a-"));
//    BOOST_CHECK_EQUAL(true, test_integer("0+a-1"));
//    BOOST_CHECK_EQUAL(true, test_integer("0+a1"));
//    BOOST_CHECK_EQUAL(true, test_integer("0+a1-"));
//    BOOST_CHECK_EQUAL(true, test_integer("01"));
//    BOOST_CHECK_EQUAL(true, test_integer("01-"));
//    BOOST_CHECK_EQUAL(true, test_integer("01-+"));
//    BOOST_CHECK_EQUAL(true, test_integer("01-+a"));
//    BOOST_CHECK_EQUAL(true, test_integer("01-a"));
//    BOOST_CHECK_EQUAL(true, test_integer("01-a+"));
//    BOOST_CHECK_EQUAL(true, test_integer("01+"));
//    BOOST_CHECK_EQUAL(true, test_integer("01+-"));
//    BOOST_CHECK_EQUAL(true, test_integer("01+-a"));
//    BOOST_CHECK_EQUAL(true, test_integer("01+a"));
//    BOOST_CHECK_EQUAL(true, test_integer("01+a-"));
//    BOOST_CHECK_EQUAL(true, test_integer("01a"));
//    BOOST_CHECK_EQUAL(true, test_integer("01a-"));
//    BOOST_CHECK_EQUAL(true, test_integer("01a-+"));
//    BOOST_CHECK_EQUAL(true, test_integer("01a+"));
//    BOOST_CHECK_EQUAL(true, test_integer("01a+-"));
//    BOOST_CHECK_EQUAL(true, test_integer("0a"));
//    BOOST_CHECK_EQUAL(true, test_integer("0a-"));
//    BOOST_CHECK_EQUAL(true, test_integer("0a-+"));
//    BOOST_CHECK_EQUAL(true, test_integer("0a-+1"));
//    BOOST_CHECK_EQUAL(true, test_integer("0a-1"));
//    BOOST_CHECK_EQUAL(true, test_integer("0a-1+"));
//    BOOST_CHECK_EQUAL(true, test_integer("0a+"));
//    BOOST_CHECK_EQUAL(true, test_integer("0a+-"));
//    BOOST_CHECK_EQUAL(true, test_integer("0a+-1"));
//    BOOST_CHECK_EQUAL(true, test_integer("0a+1"));
//    BOOST_CHECK_EQUAL(true, test_integer("0a+1-"));
//    BOOST_CHECK_EQUAL(true, test_integer("0a1"));
//    BOOST_CHECK_EQUAL(true, test_integer("0a1-"));
//    BOOST_CHECK_EQUAL(true, test_integer("0a1-+"));
//    BOOST_CHECK_EQUAL(true, test_integer("0a1+"));
//    BOOST_CHECK_EQUAL(true, test_integer("0a1+-"));
//    BOOST_CHECK_EQUAL(true, test_integer("1"));
//    BOOST_CHECK_EQUAL(true, test_integer("1-"));
//    BOOST_CHECK_EQUAL(true, test_integer("1-+"));
//    BOOST_CHECK_EQUAL(true, test_integer("1-+0"));
//    BOOST_CHECK_EQUAL(true, test_integer("1-+0a"));
//    BOOST_CHECK_EQUAL(true, test_integer("1-+a"));
//    BOOST_CHECK_EQUAL(true, test_integer("1-+a0"));
//    BOOST_CHECK_EQUAL(true, test_integer("1-0"));
//    BOOST_CHECK_EQUAL(true, test_integer("1-0+"));
//    BOOST_CHECK_EQUAL(true, test_integer("1-0+a"));
//    BOOST_CHECK_EQUAL(true, test_integer("1-0a"));
//    BOOST_CHECK_EQUAL(true, test_integer("1-0a+"));
//    BOOST_CHECK_EQUAL(true, test_integer("1-a"));
//    BOOST_CHECK_EQUAL(true, test_integer("1-a+"));
//    BOOST_CHECK_EQUAL(true, test_integer("1-a+0"));
//    BOOST_CHECK_EQUAL(true, test_integer("1-a0"));
//    BOOST_CHECK_EQUAL(true, test_integer("1-a0+"));
//    BOOST_CHECK_EQUAL(true, test_integer("1+"));
//    BOOST_CHECK_EQUAL(true, test_integer("1+-"));
//    BOOST_CHECK_EQUAL(true, test_integer("1+-0"));
//    BOOST_CHECK_EQUAL(true, test_integer("1+-0a"));
//    BOOST_CHECK_EQUAL(true, test_integer("1+-a"));
//    BOOST_CHECK_EQUAL(true, test_integer("1+-a0"));
//    BOOST_CHECK_EQUAL(true, test_integer("1+0"));
//    BOOST_CHECK_EQUAL(true, test_integer("1+0-"));
//    BOOST_CHECK_EQUAL(true, test_integer("1+0-a"));
//    BOOST_CHECK_EQUAL(true, test_integer("1+0a"));
//    BOOST_CHECK_EQUAL(true, test_integer("1+0a-"));
//    BOOST_CHECK_EQUAL(true, test_integer("1+a"));
//    BOOST_CHECK_EQUAL(true, test_integer("1+a-"));
//    BOOST_CHECK_EQUAL(true, test_integer("1+a-0"));
//    BOOST_CHECK_EQUAL(true, test_integer("1+a0"));
//    BOOST_CHECK_EQUAL(true, test_integer("1+a0-"));
//    BOOST_CHECK_EQUAL(true, test_integer("10"));
//    BOOST_CHECK_EQUAL(true, test_integer("10-"));
//    BOOST_CHECK_EQUAL(true, test_integer("10-+"));
//    BOOST_CHECK_EQUAL(true, test_integer("10-+a"));
//    BOOST_CHECK_EQUAL(true, test_integer("10-a"));
//    BOOST_CHECK_EQUAL(true, test_integer("10-a+"));
//    BOOST_CHECK_EQUAL(true, test_integer("10+"));
//    BOOST_CHECK_EQUAL(true, test_integer("10+-"));
//    BOOST_CHECK_EQUAL(true, test_integer("10+-a"));
//    BOOST_CHECK_EQUAL(true, test_integer("10+a"));
//    BOOST_CHECK_EQUAL(true, test_integer("10+a-"));
//    BOOST_CHECK_EQUAL(true, test_integer("10a"));
//    BOOST_CHECK_EQUAL(true, test_integer("10a-"));
//    BOOST_CHECK_EQUAL(true, test_integer("10a-+"));
//    BOOST_CHECK_EQUAL(true, test_integer("10a+"));
//    BOOST_CHECK_EQUAL(true, test_integer("10a+-"));
//    BOOST_CHECK_EQUAL(true, test_integer("1a"));
//    BOOST_CHECK_EQUAL(true, test_integer("1a-"));
//    BOOST_CHECK_EQUAL(true, test_integer("1a-+"));
//    BOOST_CHECK_EQUAL(true, test_integer("1a-+0"));
//    BOOST_CHECK_EQUAL(true, test_integer("1a-0"));
//    BOOST_CHECK_EQUAL(true, test_integer("1a-0+"));
//    BOOST_CHECK_EQUAL(true, test_integer("1a+"));
//    BOOST_CHECK_EQUAL(true, test_integer("1a+-"));
//    BOOST_CHECK_EQUAL(true, test_integer("1a+-0"));
//    BOOST_CHECK_EQUAL(true, test_integer("1a+0"));
//    BOOST_CHECK_EQUAL(true, test_integer("1a+0-"));
//    BOOST_CHECK_EQUAL(true, test_integer("1a0"));
//    BOOST_CHECK_EQUAL(true, test_integer("1a0-"));
//    BOOST_CHECK_EQUAL(true, test_integer("1a0-+"));
//    BOOST_CHECK_EQUAL(true, test_integer("1a0+"));
//    BOOST_CHECK_EQUAL(true, test_integer("1a0+-"));
//    BOOST_CHECK_EQUAL(false, test_integer("a"));
//    BOOST_CHECK_EQUAL(false, test_integer("a-"));
//    BOOST_CHECK_EQUAL(false, test_integer("a-+"));
//    BOOST_CHECK_EQUAL(false, test_integer("a-+0"));
//    BOOST_CHECK_EQUAL(false, test_integer("a-+01"));
//    BOOST_CHECK_EQUAL(false, test_integer("a-+1"));
//    BOOST_CHECK_EQUAL(false, test_integer("a-+10"));
//    BOOST_CHECK_EQUAL(false, test_integer("a-0"));
//    BOOST_CHECK_EQUAL(false, test_integer("a-0+"));
//    BOOST_CHECK_EQUAL(false, test_integer("a-0+1"));
//    BOOST_CHECK_EQUAL(false, test_integer("a-01"));
//    BOOST_CHECK_EQUAL(false, test_integer("a-01+"));
//    BOOST_CHECK_EQUAL(false, test_integer("a-1"));
//    BOOST_CHECK_EQUAL(false, test_integer("a-1+"));
//    BOOST_CHECK_EQUAL(false, test_integer("a-1+0"));
//    BOOST_CHECK_EQUAL(false, test_integer("a-10"));
//    BOOST_CHECK_EQUAL(false, test_integer("a-10+"));
//    BOOST_CHECK_EQUAL(false, test_integer("a+"));
//    BOOST_CHECK_EQUAL(false, test_integer("a+-"));
//    BOOST_CHECK_EQUAL(false, test_integer("a+-0"));
//    BOOST_CHECK_EQUAL(false, test_integer("a+-01"));
//    BOOST_CHECK_EQUAL(false, test_integer("a+-1"));
//    BOOST_CHECK_EQUAL(false, test_integer("a+-10"));
//    BOOST_CHECK_EQUAL(false, test_integer("a+0"));
//    BOOST_CHECK_EQUAL(false, test_integer("a+0-"));
//    BOOST_CHECK_EQUAL(false, test_integer("a+0-1"));
//    BOOST_CHECK_EQUAL(false, test_integer("a+01"));
//    BOOST_CHECK_EQUAL(false, test_integer("a+01-"));
//    BOOST_CHECK_EQUAL(false, test_integer("a+1"));
//    BOOST_CHECK_EQUAL(false, test_integer("a+1-"));
//    BOOST_CHECK_EQUAL(false, test_integer("a+1-0"));
//    BOOST_CHECK_EQUAL(false, test_integer("a+10"));
//    BOOST_CHECK_EQUAL(false, test_integer("a+10-"));
//    BOOST_CHECK_EQUAL(false, test_integer("a0"));
//    BOOST_CHECK_EQUAL(false, test_integer("a0-"));
//    BOOST_CHECK_EQUAL(false, test_integer("a0-+"));
//    BOOST_CHECK_EQUAL(false, test_integer("a0-+1"));
//    BOOST_CHECK_EQUAL(false, test_integer("a0-1"));
//    BOOST_CHECK_EQUAL(false, test_integer("a0-1+"));
//    BOOST_CHECK_EQUAL(false, test_integer("a0+"));
//    BOOST_CHECK_EQUAL(false, test_integer("a0+-"));
//    BOOST_CHECK_EQUAL(false, test_integer("a0+-1"));
//    BOOST_CHECK_EQUAL(false, test_integer("a0+1"));
//    BOOST_CHECK_EQUAL(false, test_integer("a0+1-"));
//    BOOST_CHECK_EQUAL(false, test_integer("a01"));
//    BOOST_CHECK_EQUAL(false, test_integer("a01-"));
//    BOOST_CHECK_EQUAL(false, test_integer("a01-+"));
//    BOOST_CHECK_EQUAL(false, test_integer("a01+"));
//    BOOST_CHECK_EQUAL(false, test_integer("a01+-"));
//    BOOST_CHECK_EQUAL(false, test_integer("a1"));
//    BOOST_CHECK_EQUAL(false, test_integer("a1-"));
//    BOOST_CHECK_EQUAL(false, test_integer("a1-+"));
//    BOOST_CHECK_EQUAL(false, test_integer("a1-+0"));
//    BOOST_CHECK_EQUAL(false, test_integer("a1-0"));
//    BOOST_CHECK_EQUAL(false, test_integer("a1-0+"));
//    BOOST_CHECK_EQUAL(false, test_integer("a1+"));
//    BOOST_CHECK_EQUAL(false, test_integer("a1+-"));
//    BOOST_CHECK_EQUAL(false, test_integer("a1+-0"));
//    BOOST_CHECK_EQUAL(false, test_integer("a1+0"));
//    BOOST_CHECK_EQUAL(false, test_integer("a1+0-"));
//    BOOST_CHECK_EQUAL(false, test_integer("a10"));
//    BOOST_CHECK_EQUAL(false, test_integer("a10-"));
//    BOOST_CHECK_EQUAL(false, test_integer("a10-+"));
//    BOOST_CHECK_EQUAL(false, test_integer("a10+"));
//    BOOST_CHECK_EQUAL(false, test_integer("a10+-"));
}