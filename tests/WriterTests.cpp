#define _CRT_SECURE_NO_WARNINGS
#include "../Writer.h"
#include "tests.hpp"
#include <cmath>

struct WriterFixture {
	std::stringstream stream;
	az::json::Writer writer;

	WriterFixture() : writer(stream) {}
};

BOOST_AUTO_TEST_SUITE(WriterTests)

BOOST_FIXTURE_TEST_CASE(write_null, WriterFixture)
{
	writer.write(az::json::Value(nullptr));
	BOOST_CHECK_EQUAL(stream.str(), "null");
}

BOOST_FIXTURE_TEST_CASE(write_bool_true, WriterFixture)
{
	writer.write(az::json::Value(true));
	BOOST_CHECK_EQUAL(stream.str(), "true");
}

BOOST_FIXTURE_TEST_CASE(write_bool_false, WriterFixture)
{
	writer.write(az::json::Value(false));
	BOOST_CHECK_EQUAL(stream.str(), "false");
}

BOOST_FIXTURE_TEST_CASE(write_positive_integer, WriterFixture)
{
	writer.write(az::json::Value(123));
	BOOST_CHECK_EQUAL(stream.str(), "123");
}

BOOST_FIXTURE_TEST_CASE(write_negative_integer, WriterFixture)
{
	writer.write(az::json::Value(-987));
	BOOST_CHECK_EQUAL(stream.str(), "-987");
}

BOOST_FIXTURE_TEST_CASE(write_positive_real, WriterFixture)
{
	writer.write(az::json::Value(3.14));
	BOOST_CHECK_EQUAL(stream.str(), "3.14");
}

BOOST_FIXTURE_TEST_CASE(write_negative_real, WriterFixture)
{
	writer.write(az::json::Value(-2.78));
	BOOST_CHECK_EQUAL(stream.str(), "-2.78");
}

BOOST_FIXTURE_TEST_CASE(write_positive_nan, WriterFixture)
{
	writer.write(az::json::Value(std::nan("")));
	BOOST_CHECK_EQUAL(stream.str(), "NaN");
}

BOOST_FIXTURE_TEST_CASE(write_negative_nan, WriterFixture)
{
	writer.write(az::json::Value(-std::nan("")));
	BOOST_CHECK_EQUAL(stream.str(), "-NaN");
}

BOOST_FIXTURE_TEST_CASE(write_positive_infinity, WriterFixture)
{
	writer.write(az::json::Value(INFINITY));
	BOOST_CHECK_EQUAL(stream.str(), "Infinity");
}

BOOST_FIXTURE_TEST_CASE(write_negative_infinity, WriterFixture)
{
	writer.write(az::json::Value(-INFINITY));
	BOOST_CHECK_EQUAL(stream.str(), "-Infinity");
}

BOOST_FIXTURE_TEST_CASE(write_scientific_real, WriterFixture)
{
	stream << std::scientific;
	writer.write(az::json::Value(0.003));
	BOOST_CHECK_EQUAL(stream.str(), "3.000000e-03");
}

BOOST_FIXTURE_TEST_CASE(write_positive_hex, WriterFixture)
{
	stream << std::hex << std::showbase;
	writer.write(az::json::Value(0xdead));
	BOOST_CHECK_EQUAL(stream.str(), "0xdead");
}

BOOST_FIXTURE_TEST_CASE(write_negative_hex, WriterFixture)
{
	stream << std::hex << std::showbase;
	writer.write(az::json::Value(-0xbeaf));
	BOOST_CHECK_EQUAL(stream.str(), "-0xbeaf");
}

BOOST_FIXTURE_TEST_CASE(write_uppercase_hex, WriterFixture)
{
	stream << std::hex << std::showbase << std::uppercase;
	writer.write(az::json::Value(int64_t(0xABBA1972)));
	BOOST_CHECK_EQUAL(stream.str(), "0XABBA1972");
}

BOOST_FIXTURE_TEST_CASE(write_string, WriterFixture)
{
	writer.write("ijhgjih");
	BOOST_CHECK_EQUAL(stream.str(), "\"ijhgjih\"");
}

BOOST_FIXTURE_TEST_CASE(write_string_with_special_chars, WriterFixture)
{
	writer.write("'\"\t\r\n");
	BOOST_CHECK_EQUAL(stream.str(), "\"\\'\\\"\\t\\r\\n\"");
}

BOOST_FIXTURE_TEST_CASE(write_string_with_unicode, WriterFixture)
{
	writer.write({std::string({char(0xE2), char(0x82), char(0xAC)}), 123});
	BOOST_CHECK_EQUAL(stream.str(), "[\"\\u20ac\",123]");
}

BOOST_FIXTURE_TEST_CASE(write_array, WriterFixture)
{
	writer.write({1, 2.78, "three"});
	BOOST_CHECK_EQUAL(stream.str(), "[1,2.78,\"three\"]");
}

BOOST_FIXTURE_TEST_CASE(write_object, WriterFixture)
{
	writer.write({{"one", 1}});
	BOOST_CHECK_EQUAL(stream.str(), "{\"one\":1}");
}

BOOST_FIXTURE_TEST_CASE(write_without_quoting, WriterFixture)
{
	writer.withoutQuoting().write({{"one", 1}});
	BOOST_CHECK_EQUAL(stream.str(), "{one:1}");
}

BOOST_FIXTURE_TEST_CASE(pretty_write_array, WriterFixture)
{
	writer.pretty().write({1, "two"});
	BOOST_CHECK_EQUAL(stream.str(), "[\n   1,\n   \"two\"\n]");
}

BOOST_FIXTURE_TEST_CASE(pretty_write_object, WriterFixture)
{
	writer.pretty().write({{"one", 1}});
	BOOST_CHECK_EQUAL(stream.str(), "{\n   \"one\": 1\n}");
}

BOOST_FIXTURE_TEST_CASE(pretty_write_with_new_line, WriterFixture)
{
	writer.withNewLine("\r\n").pretty().write({123});
	BOOST_CHECK_EQUAL(stream.str(), "[\r\n   123\r\n]");
}

BOOST_FIXTURE_TEST_CASE(pretty_write_with_indent_size, WriterFixture)
{
	writer.withIndentSize(5).pretty().write({3.14});
	BOOST_CHECK_EQUAL(stream.str(), "[\n     3.14\n]");
}

BOOST_FIXTURE_TEST_CASE(pretty_write_with_indent_char, WriterFixture)
{
	writer.withIndentChar('\t').withIndentSize(1).pretty().write({nullptr});
	BOOST_CHECK_EQUAL(stream.str(), "[\n\tnull\n]");
}

BOOST_FIXTURE_TEST_CASE(pretty_write_with_left_margin, WriterFixture)
{
	writer.withLeftMargin(1).withIndentSize(2).pretty().write({true});
	BOOST_CHECK_EQUAL(stream.str(), " [\n   true\n ]");
}

BOOST_AUTO_TEST_SUITE_END()
