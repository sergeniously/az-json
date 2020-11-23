#include "test.hpp"
#include <json/Value.h>

BOOST_AUTO_TEST_SUITE(ValueTests)

BOOST_AUTO_TEST_CASE(make_null)
{
	az::json::Value json;
	BOOST_CHECK_EQUAL(json.getType(), az::json::Value::Type::Null);
	BOOST_CHECK(json.isNull());
	BOOST_CHECK(json.empty());
	BOOST_CHECK_EQUAL(json.size(), 0);
	BOOST_CHECK(!json.has(1));
	BOOST_CHECK(!json.has("key"));
	BOOST_CHECK_EQUAL(json.stringify(), "null");
}

BOOST_AUTO_TEST_CASE(make_null_from_nullptr)
{
	az::json::Value json(nullptr);
	BOOST_CHECK(json.isNull());
}

BOOST_AUTO_TEST_CASE(make_bool)
{
	az::json::Value json(true);
	BOOST_CHECK(json.isBool());
	BOOST_CHECK(json.asBool());
	BOOST_CHECK(!json.empty());
}

BOOST_AUTO_TEST_CASE(make_integer)
{
	az::json::Value json(123);
	BOOST_CHECK(json.isInteger());
	BOOST_CHECK_EQUAL(json.asInteger(), 123);
	BOOST_CHECK(!json.empty());
}

BOOST_AUTO_TEST_CASE(make_real)
{
	az::json::Value json(3.14);
	BOOST_CHECK(json.isReal());
	BOOST_CHECK_EQUAL(json.asReal(), 3.14);
	BOOST_CHECK(!json.empty());
}

BOOST_AUTO_TEST_CASE(make_string)
{
	az::json::Value json("json");
	BOOST_CHECK(json.isString());
	BOOST_CHECK_EQUAL(json.asString(), "json");
	BOOST_CHECK(!json.empty());
	BOOST_CHECK_EQUAL(json.size(), 4);
}

BOOST_AUTO_TEST_CASE(make_array_from_initlist)
{
	az::json::Value json = {
		false, 123, 3.14, "json"
	};
	BOOST_REQUIRE(json.isArray());
	BOOST_REQUIRE_EQUAL(json.size(), 4);
	BOOST_CHECK_EQUAL(json[0].asBool(), false);
	BOOST_CHECK_EQUAL(json[1].asInteger(), 123);
	BOOST_CHECK_EQUAL(json[2].asReal(), 3.14);
	BOOST_CHECK_EQUAL(json[3].asString(), "json");
}

BOOST_AUTO_TEST_CASE(make_array_from_range)
{
	std::list<az::json::Value> range = {
		true, 123, -2.78, "json"
	};
	az::json::Value json(range.begin(), range.end());
	BOOST_REQUIRE(json.isArray());
	BOOST_REQUIRE_EQUAL(json.size(), 4);
	BOOST_CHECK_EQUAL(json[0].asBool(), true);
	BOOST_CHECK_EQUAL(json[1].asInteger(), 123);
	BOOST_CHECK_EQUAL(json[2].asReal(), -2.78);
	BOOST_CHECK_EQUAL(json[3].asString(), "json");
}

BOOST_AUTO_TEST_CASE(make_array_automatically)
{
	az::json::Value json;
	json[3] = "third";
	BOOST_REQUIRE(json.isArray());
	BOOST_CHECK_EQUAL(json.size(), 4);
	BOOST_CHECK(json.has(3));
}

BOOST_AUTO_TEST_CASE(append_to_array)
{
	az::json::Value json = {1};
	BOOST_REQUIRE_EQUAL(json.size(), 1);
	json.append("two");
	BOOST_REQUIRE_EQUAL(json.size(), 2);
	BOOST_CHECK_EQUAL(json[1].asString(), "two");
}

BOOST_AUTO_TEST_CASE(append_to_not_array)
{
	az::json::Value json = 123;
	json.append(321);
	BOOST_REQUIRE_EQUAL(json.size(), 1);
	BOOST_CHECK_EQUAL(json[0].asInteger(), 321);
}

BOOST_AUTO_TEST_CASE(make_object_from_initlist)
{
	az::json::Value json = {
		{"null", nullptr},
		{"bool", true},
		{"integer", -123},
		{"real", 3.1415},
		{"string", "json"},
		{"array", {false, 123, 3.14, "json"}},
		{"object", {
			{"one", 1},
			{"two", 2.0}
		}}
	};

	BOOST_CHECK_EQUAL(json.getType(), az::json::Value::Type::Object);
	BOOST_CHECK(json.isObject());
	BOOST_CHECK(!json.empty());
	BOOST_CHECK_EQUAL(json.size(), 7);

	BOOST_CHECK(json.has("null"));
	BOOST_CHECK(json.has("bool"));
	BOOST_CHECK(json.has("integer"));
	BOOST_CHECK(json.has("real"));
	BOOST_CHECK(json.has("string"));
	BOOST_CHECK(json.has("array"));
	BOOST_CHECK(json.has("object"));

	BOOST_CHECK_EQUAL(json["null"].getType(), az::json::Value::Type::Null);
	BOOST_CHECK_EQUAL(json["bool"].asBool(), true);
	BOOST_CHECK_EQUAL(json["integer"].asInteger(), -123);
	BOOST_CHECK_EQUAL(json["real"].asReal(), 3.1415);
	BOOST_CHECK_EQUAL(json["string"].asString(), "json");
	BOOST_CHECK_EQUAL(json["array"].size(), 4);
	BOOST_CHECK_EQUAL(json["object"].size(), 2);

	BOOST_CHECK(!json.has("none"));
	BOOST_CHECK(!json.has(2));
}

BOOST_AUTO_TEST_CASE(make_object_automatically)
{
	az::json::Value json;
	json["json"] = 5;
	BOOST_REQUIRE(json.isObject());
	BOOST_CHECK_EQUAL(json.size(), 1);
	BOOST_CHECK(json.has("json"));
}

BOOST_AUTO_TEST_CASE(swap_values)
{
	az::json::Value first = 123;
	az::json::Value second = "two";
	first.swap(second);

	BOOST_CHECK_EQUAL(first.asString(), "two");
	BOOST_CHECK_EQUAL(second.asInteger(), 123);
}

BOOST_AUTO_TEST_CASE(get_default)
{
	az::json::Value json = {
		{"yes", 123}
	};
	auto value = json.get("no", 321);
	BOOST_REQUIRE_EQUAL(value.asInteger(), 321);
}

BOOST_AUTO_TEST_SUITE_END()
