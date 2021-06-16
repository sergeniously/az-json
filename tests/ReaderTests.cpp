#define _CRT_SECURE_NO_WARNINGS
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "Reader.h"
#include <cmath>

struct ReaderFixture {
	az::json::Value json;
	az::json::Reader reader;
	ReaderFixture()
		: reader(json) {}
	void parse(const char* text) {
		BOOST_REQUIRE_NO_THROW(reader.parse(text));
		BOOST_REQUIRE(!reader.hasErrors());
	}
};

BOOST_AUTO_TEST_SUITE(ReaderTests)

BOOST_FIXTURE_TEST_CASE(parse_null, ReaderFixture)
{
	parse("null");
	BOOST_CHECK(json.isNull());
}

BOOST_FIXTURE_TEST_CASE(parse_bools, ReaderFixture)
{
	parse("true");
	BOOST_CHECK(json.isBool());
	BOOST_CHECK_EQUAL(json.asBool(), true);

	parse("false");
	BOOST_CHECK(json.isBool());
	BOOST_CHECK_EQUAL(json.asBool(), false);
}

BOOST_FIXTURE_TEST_CASE(parse_integers, ReaderFixture)
{
	parse("123");
	BOOST_CHECK(json.isInteger());
	BOOST_CHECK_EQUAL(json.asInteger(), 123);
	BOOST_CHECK(!json.isNegative());

	parse("-321");
	BOOST_CHECK(json.isInteger());
	BOOST_CHECK_EQUAL(json.asInteger(), -321);
	BOOST_CHECK(json.isNegative());

	parse("+2020");
	BOOST_CHECK(json.isInteger());
	BOOST_CHECK_EQUAL(json.asInteger(), 2020);
	BOOST_CHECK(!json.isNegative());
}

BOOST_FIXTURE_TEST_CASE(parse_hexadecimals, ReaderFixture)
{
	parse("0xabc123");
	BOOST_CHECK(json.isInteger());
	BOOST_CHECK_EQUAL(json.asInteger(), 0xABC123);
	BOOST_CHECK(!json.isNegative());

	parse("-0x123ABC");
	BOOST_CHECK(json.isInteger());
	BOOST_CHECK_EQUAL(json.asInteger(), -0x123ABC);
	BOOST_CHECK(json.isNegative());

	parse("+0x0000Abba");
	BOOST_CHECK(json.isInteger());
	BOOST_CHECK_EQUAL(json.asInteger(), 0x0000ABBA);
	BOOST_CHECK(!json.isNegative());
}

BOOST_FIXTURE_TEST_CASE(parse_reals, ReaderFixture)
{
	parse("3.14");
	BOOST_CHECK(json.isReal());
	BOOST_CHECK_EQUAL(json.asReal(), 3.14);
	BOOST_CHECK(!json.isNegative());

	parse("-2.78");
	BOOST_CHECK(json.isReal());
	BOOST_CHECK_EQUAL(json.asReal(), -2.78);
	BOOST_CHECK(json.isNegative());

	parse("+0.69");
	BOOST_CHECK(json.isReal());
	BOOST_CHECK_EQUAL(json.asReal(), 0.69);
	BOOST_CHECK(!json.isNegative());

	parse(".123");
	BOOST_CHECK(json.isReal());
	BOOST_CHECK_EQUAL(json.asReal(), 0.123);
	BOOST_CHECK(!json.isNegative());

	parse("-.123");
	BOOST_CHECK(json.isReal());
	BOOST_CHECK_EQUAL(json.asReal(), -0.123);
	BOOST_CHECK(json.isNegative());

	parse("+.123");
	BOOST_CHECK(json.isReal());
	BOOST_CHECK_EQUAL(json.asReal(), 0.123);
	BOOST_CHECK(!json.isNegative());

	parse("123.");
	BOOST_CHECK(json.isReal());
	BOOST_CHECK_EQUAL(json.asReal(), 123.0);
	BOOST_CHECK(!json.isNegative());

	parse("1e-6");
	BOOST_CHECK(json.isReal());
	BOOST_CHECK_EQUAL(json.asReal(), 1e-6);
	BOOST_CHECK(!json.isNegative());

	parse("+2E-6");
	BOOST_CHECK(json.isReal());
	BOOST_CHECK_EQUAL(json.asReal(), 2e-6);
	BOOST_CHECK(!json.isNegative());

	parse("-3e+3");
	BOOST_CHECK(json.isReal());
	BOOST_CHECK_EQUAL(json.asReal(), -3e+3);
	BOOST_CHECK(json.isNegative());

	parse("4E5");
	BOOST_CHECK(json.isReal());
	BOOST_CHECK_EQUAL(json.asReal(), 4e5);
	BOOST_CHECK(!json.isNegative());

	parse("NaN");
	BOOST_CHECK(json.isReal());
	BOOST_CHECK(std::isnan(json.asReal()));
	BOOST_CHECK(!json.isNegative());

	parse("-NaN");
	BOOST_CHECK(json.isReal());
	BOOST_CHECK(std::isnan(json.asReal()));
	BOOST_CHECK(json.isNegative());

	parse("+NaN");
	BOOST_CHECK(json.isReal());
	BOOST_CHECK(std::isnan(json.asReal()));
	BOOST_CHECK(!json.isNegative());

	parse("Infinity");
	BOOST_CHECK(json.isReal());
	BOOST_CHECK(std::isinf(json.asReal()));
	BOOST_CHECK(!json.isNegative());

	parse("-Infinity");
	BOOST_CHECK(json.isReal());
	BOOST_CHECK(std::isinf(json.asReal()));
	BOOST_CHECK(json.isNegative());

	parse("+Infinity");
	BOOST_CHECK(json.isReal());
	BOOST_CHECK(std::isinf(json.asReal()));
	BOOST_CHECK(!json.isNegative());
}

BOOST_FIXTURE_TEST_CASE(parse_strings, ReaderFixture)
{
	parse("\"json\"");
	BOOST_CHECK(json.isString());
	BOOST_CHECK_EQUAL(json.asString(), "json");

	parse("'jsoff'");
	BOOST_CHECK(json.isString());
	BOOST_CHECK_EQUAL(json.asString(), "jsoff");

	parse("json5");
	BOOST_CHECK(json.isString());
	BOOST_CHECK_EQUAL(json.asString(), "json5");

	parse("'string with no\\\n new line'");
	BOOST_CHECK(json.isString());
	BOOST_CHECK_EQUAL(json.asString(), "string with no new line");

	parse("'\\u00A9 \\u00abJSON\\u00Bb'");
	BOOST_CHECK(json.isString());
	BOOST_CHECK_EQUAL(json.asString(), "© «JSON»");
	BOOST_CHECK_EQUAL(json.asWideString(), std::wstring({0xA9, ' ', 0xAB, 'J', 'S', 'O', 'N', 0xBB}));
}

BOOST_FIXTURE_TEST_CASE(parse_array, ReaderFixture)
{
	parse("[null,true,123,3.14,'json',[],{}]");
	BOOST_REQUIRE(json.isArray());
	BOOST_REQUIRE_EQUAL(json.size(), 7);

	BOOST_REQUIRE(json[0].isNull());
	BOOST_REQUIRE(json[1].isBool());
	BOOST_REQUIRE(json[2].isInteger());
	BOOST_REQUIRE(json[3].isReal());
	BOOST_REQUIRE(json[4].isString());
	BOOST_REQUIRE(json[5].isArray());
	BOOST_REQUIRE(json[6].isObject());

	BOOST_CHECK_EQUAL(json[1].asBool(), true);
	BOOST_CHECK_EQUAL(json[2].asInteger(), 123);
	BOOST_CHECK_EQUAL(json[3].asReal(), 3.14);
	BOOST_CHECK_EQUAL(json[4].asString(), "json");
	BOOST_CHECK_EQUAL(json[5].size(), 0);
	BOOST_CHECK_EQUAL(json[6].size(), 0);
}

BOOST_FIXTURE_TEST_CASE(parse_object, ReaderFixture)
{
	parse("{null:null,'bool':false,\"integer\":123,real:2.78,'string':'json',array:[],object:{}}");
	BOOST_REQUIRE(json.isObject());
	BOOST_REQUIRE_EQUAL(json.size(), 7);

	BOOST_REQUIRE(json["null"].isNull());
	BOOST_REQUIRE(json["bool"].isBool());
	BOOST_REQUIRE(json["integer"].isInteger());
	BOOST_REQUIRE(json["real"].isReal());
	BOOST_REQUIRE(json["string"].isString());
	BOOST_REQUIRE(json["array"].isArray());
	BOOST_REQUIRE(json["object"].isObject());

	BOOST_CHECK_EQUAL(json["bool"].asBool(), false);
	BOOST_CHECK_EQUAL(json["integer"].asInteger(), 123);
	BOOST_CHECK_EQUAL(json["real"].asReal(), 2.78);
	BOOST_CHECK_EQUAL(json["string"].asString(), "json");
	BOOST_CHECK_EQUAL(json["array"].size(), 0);
	BOOST_CHECK_EQUAL(json["object"].size(), 0);
}

BOOST_FIXTURE_TEST_CASE(parse_from_stream, ReaderFixture)
{
	std::istringstream stream("{json : 'five'}");
	BOOST_REQUIRE_NO_THROW(reader.strictly().parse(stream));
	BOOST_REQUIRE(json.isObject());
	BOOST_REQUIRE(json.has("json"));
	BOOST_CHECK_EQUAL(json["json"].asString(), "five");
}

BOOST_FIXTURE_TEST_CASE(parse_from_range, ReaderFixture)
{
	std::string text = " [null, true, 0xAD] ";
	BOOST_REQUIRE_NO_THROW(reader.parse(text.begin(), text.end()));
	BOOST_REQUIRE(json.isArray());
	BOOST_REQUIRE_EQUAL(json.size(), 3);
	BOOST_CHECK(json[0].isNull());
	BOOST_CHECK_EQUAL(json[1].asBool(), true);
	BOOST_CHECK_EQUAL(json[2].asInteger(), 0xAD);
}

BOOST_FIXTURE_TEST_CASE(parse_array_with_trailing_comma, ReaderFixture)
{
	parse("[one, two,]");
	BOOST_REQUIRE(json.isArray());
	BOOST_CHECK_EQUAL(json.size(), 2);
}

BOOST_FIXTURE_TEST_CASE(parse_object_with_trailing_comma, ReaderFixture)
{
	parse("{one:1, two:2,}");
	BOOST_REQUIRE(json.isObject());
	BOOST_CHECK_EQUAL(json.size(), 2);
}

BOOST_FIXTURE_TEST_CASE(parse_with_comments, ReaderFixture)
{
	auto text = R"({
		// single line comment
		"json": -5.0 /*
			multi line comment
		*/
	})";
	parse(text);
	BOOST_REQUIRE(json.isObject());
	BOOST_REQUIRE(json.has("json"));
	BOOST_CHECK_EQUAL(json["json"].asReal(), -5.0);
}

BOOST_FIXTURE_TEST_CASE(double_parse_to_same_target, ReaderFixture)
{
	parse("{year: 2020}");
	BOOST_REQUIRE(json.isObject());
	parse("[year, 2020]");
	BOOST_CHECK(json.isArray());
}

BOOST_FIXTURE_TEST_CASE(parse_with_no_throws, ReaderFixture)
{
	BOOST_REQUIRE_NO_THROW(reader.withNoThrows().parse("{bad}"));
	BOOST_CHECK(reader.hasErrors());
}

BOOST_FIXTURE_TEST_CASE(strictly_parse, ReaderFixture)
{
	BOOST_REQUIRE_THROW(reader.strictly().parse("{json:5}..."), az::json::Error);
}

BOOST_FIXTURE_TEST_CASE(not_strictly_parse, ReaderFixture)
{
	BOOST_REQUIRE_NO_THROW(reader.strictly(false).parse("{json:5}..."));
}

BOOST_FIXTURE_TEST_CASE(parse_invalid_object, ReaderFixture)
{
	BOOST_REQUIRE_THROW(reader.parse("{json = 5}"), az::json::Error);
	BOOST_REQUIRE(reader.hasErrors());
	auto error = reader.getLastError();
	BOOST_CHECK_EQUAL(error.line(), 1);
	BOOST_CHECK_EQUAL(error.column(), 6);
}

BOOST_FIXTURE_TEST_CASE(parse_invalid_array, ReaderFixture)
{
	BOOST_REQUIRE_THROW(reader.parse("[\n  json; 5\n]"), az::json::Error);
	BOOST_REQUIRE(reader.hasErrors());
	auto error = reader.getLastError();
	BOOST_CHECK_EQUAL(error.line(), 2);
	BOOST_CHECK_EQUAL(error.column(), 7);
}

BOOST_FIXTURE_TEST_CASE(parse_with_unicode_whitespaces, ReaderFixture)
{
	std::string text = {
		char(0xEF), char(0xBB), char(0xBF), // BOM
		'{', char(0xE2), char(0x80), char(0xA8), // Line separator
			'j', 's', 'o', 'n', ':', '5', char(0xE2), char(0x80), char(0xA9), // Paragraph
		'}'
	};
	BOOST_REQUIRE_NO_THROW(reader.parse(text));
	BOOST_REQUIRE(json.isObject());
	BOOST_REQUIRE(json.has("json"));
	BOOST_CHECK_EQUAL(json["json"].asInteger(), 5);
}

BOOST_AUTO_TEST_CASE(parse_by_literal)
{
	auto json = "{json:5}"_json;
	BOOST_REQUIRE(json.isObject());
	BOOST_CHECK_EQUAL(json["json"].asInteger(), 5);
}

/*
BOOST_FIXTURE_TEST_CASE(parse_input, ReaderFixture)
{
	std::cout << "Input JSON: ";
	BOOST_REQUIRE_NO_THROW(reader.parse(std::cin));
}
*/

/*
BOOST_AUTO_TEST_CASE(parse_file)
{
	auto file = std::fopen("test.json", "r");

	az::json::Value json;
	try {
		az::json::Reader(json).parse(file);
	} catch (const std::exception& error) {
		puts(error.what());
	}

	std::fclose(file);
}*/

BOOST_AUTO_TEST_SUITE_END()
