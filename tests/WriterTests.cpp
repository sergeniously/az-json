#define _CRT_SECURE_NO_WARNINGS
#include "../Writer.h"
#include "tests.hpp"

BOOST_AUTO_TEST_SUITE(WriterTests)

BOOST_AUTO_TEST_CASE(write_null)
{
	std::stringstream stream;
	az::json::Writer(stream).write(az::json::Value(nullptr));
	BOOST_CHECK_EQUAL(stream.str(), "null");
}

BOOST_AUTO_TEST_CASE(write_bool_true)
{
	std::stringstream stream;
	az::json::Writer(stream).write(az::json::Value(true));
	BOOST_CHECK_EQUAL(stream.str(), "true");
}

BOOST_AUTO_TEST_CASE(write_bool_false)
{
	std::stringstream stream;
	az::json::Writer(stream).write(az::json::Value(false));
	BOOST_CHECK_EQUAL(stream.str(), "false");
}

BOOST_AUTO_TEST_CASE(write_positive_integer)
{
	std::stringstream stream;
	az::json::Writer(stream).write(az::json::Value(123));
	BOOST_CHECK_EQUAL(stream.str(), "123");
}

BOOST_AUTO_TEST_CASE(write_negative_integer)
{
	std::stringstream stream;
	az::json::Writer(stream).write(az::json::Value(-987));
	BOOST_CHECK_EQUAL(stream.str(), "-987");
}

BOOST_AUTO_TEST_CASE(write_positive_real)
{
	std::stringstream stream;
	az::json::Writer(stream).write(az::json::Value(3.14));
	BOOST_CHECK_EQUAL(stream.str(), "3.14");
}

BOOST_AUTO_TEST_CASE(write_negative_real)
{
	std::stringstream stream;
	az::json::Writer(stream).write(az::json::Value(-2.78));
	BOOST_CHECK_EQUAL(stream.str(), "-2.78");
}

BOOST_AUTO_TEST_CASE(write_positive_nan)
{
	std::stringstream stream;
	az::json::Writer(stream).write(az::json::Value(std::stod("nan")));
	BOOST_CHECK_EQUAL(stream.str(), "NaN");
}

BOOST_AUTO_TEST_CASE(write_negative_nan)
{
	std::stringstream stream;
	az::json::Writer(stream).write(az::json::Value(std::stod("-nan")));
	BOOST_CHECK_EQUAL(stream.str(), "-NaN");
}

BOOST_AUTO_TEST_CASE(write_positive_infinity)
{
	std::stringstream stream;
	az::json::Writer(stream).write(az::json::Value(std::stod("infinity")));
	BOOST_CHECK_EQUAL(stream.str(), "Infinity");
}

BOOST_AUTO_TEST_CASE(write_negative_infinity)
{
	std::stringstream stream;
	az::json::Writer(stream).write(az::json::Value(std::stod("-infinity")));
	BOOST_CHECK_EQUAL(stream.str(), "-Infinity");
}

BOOST_AUTO_TEST_CASE(write_scientific_real)
{
	std::stringstream stream;
	stream << std::scientific;
	az::json::Writer(stream).write(az::json::Value(0.003));
	BOOST_CHECK_EQUAL(stream.str(), "3.000000e-03");
}

BOOST_AUTO_TEST_CASE(write_positive_hex)
{
	std::stringstream stream;
	stream << std::hex << std::showbase;
	az::json::Writer(stream).write(az::json::Value(0xdead));
	BOOST_CHECK_EQUAL(stream.str(), "0xdead");
}

BOOST_AUTO_TEST_CASE(write_negative_hex)
{
	std::stringstream stream;
	stream << std::hex << std::showbase;
	az::json::Writer(stream).write(az::json::Value(-0xbeaf));
	BOOST_CHECK_EQUAL(stream.str(), "-0xbeaf");
}

BOOST_AUTO_TEST_CASE(write_uppercase_hex)
{
	std::stringstream stream;
	stream << std::hex << std::showbase << std::uppercase;
	az::json::Writer(stream).write(az::json::Value(int64_t(0xABBA1972)));
	BOOST_CHECK_EQUAL(stream.str(), "0XABBA1972");
}

BOOST_AUTO_TEST_CASE(write_string)
{
	std::stringstream stream;
	az::json::Writer(stream).write("ijhgjih");
	BOOST_CHECK_EQUAL(stream.str(), "\"ijhgjih\"");
}

BOOST_AUTO_TEST_CASE(write_string_with_special_chars)
{
	std::stringstream stream;
	az::json::Writer(stream).write("'\"\t\r\n");
	BOOST_CHECK_EQUAL(stream.str(), "\"\\'\\\"\\t\\r\\n\"");
}

BOOST_AUTO_TEST_CASE(write_string_with_unicode)
{
	std::stringstream stream;
	az::json::Writer(stream).write({std::string({char(0xE2), char(0x82), char(0xAC)}), 123});
	BOOST_CHECK_EQUAL(stream.str(), "[\"\\u20ac\",123]");
}

BOOST_AUTO_TEST_CASE(write_array)
{
	std::stringstream stream;
	az::json::Writer(stream).write({1, 2.78, "three"});
	BOOST_CHECK_EQUAL(stream.str(), "[1,2.78,\"three\"]");
}

BOOST_AUTO_TEST_CASE(write_object)
{
	std::stringstream stream;
	az::json::Writer(stream).write({{"one", 1}});
	BOOST_CHECK_EQUAL(stream.str(), "{\"one\":1}");
}

BOOST_AUTO_TEST_CASE(write_without_quoting)
{
	std::stringstream stream;
	az::json::Writer(stream).withoutQuoting().write({{"one", 1}});
	BOOST_CHECK_EQUAL(stream.str(), "{one:1}");
}

BOOST_AUTO_TEST_CASE(pretty_write_array)
{
	std::stringstream stream;
	az::json::Writer(stream).pretty().write({1, "two"});
	BOOST_CHECK_EQUAL(stream.str(), "[\n   1,\n   \"two\"\n]");
}

BOOST_AUTO_TEST_CASE(pretty_write_object)
{
	std::stringstream stream;
	az::json::Writer(stream).pretty().write({{"one", 1}});
	BOOST_CHECK_EQUAL(stream.str(), "{\n   \"one\": 1\n}");
}

BOOST_AUTO_TEST_CASE(pretty_write_with_new_line)
{
	std::stringstream stream;
	az::json::Writer(stream).withNewLine("\r\n").pretty().write({123});
	BOOST_CHECK_EQUAL(stream.str(), "[\r\n   123\r\n]");
}

BOOST_AUTO_TEST_CASE(pretty_write_with_indent_size)
{
	std::stringstream stream;
	az::json::Writer(stream).withIndentSize(5).pretty().write({3.14});
	BOOST_CHECK_EQUAL(stream.str(), "[\n     3.14\n]");
}

BOOST_AUTO_TEST_CASE(pretty_write_with_indent_char)
{
	std::stringstream stream;
	az::json::Writer(stream).withIndentChar('\t').withIndentSize(1).pretty().write({nullptr});
	BOOST_CHECK_EQUAL(stream.str(), "[\n\tnull\n]");
}

BOOST_AUTO_TEST_CASE(pretty_write_with_left_margin)
{
	std::stringstream stream;
	az::json::Writer(stream).withLeftMargin(1).withIndentSize(2).pretty().write({true});
	BOOST_CHECK_EQUAL(stream.str(), " [\n   true\n ]");
}

BOOST_AUTO_TEST_SUITE_END()
