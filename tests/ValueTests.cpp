#include <boost/test/unit_test.hpp>
#include "Value.h"

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

BOOST_AUTO_TEST_CASE(make_null_by_default)
{
	az::json::Value json;
	BOOST_CHECK(json.isNull());
}

BOOST_AUTO_TEST_CASE(make_null_from_nullptr)
{
	az::json::Value json(nullptr);
	BOOST_CHECK(json.isNull());
}

BOOST_AUTO_TEST_CASE(make_bool_true)
{
	az::json::Value json(true);
	BOOST_CHECK(json.isBool());
	BOOST_CHECK_EQUAL(json.asBool(), true);
	BOOST_CHECK(!json.empty());
}

BOOST_AUTO_TEST_CASE(make_bool_false)
{
	bool value = false;
	az::json::Value json(value);
	BOOST_CHECK(json.isBool());
	BOOST_CHECK_EQUAL(json.asBool(), false);
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

BOOST_AUTO_TEST_CASE(make_string_from_c_string)
{
	az::json::Value json("json");
	BOOST_CHECK(json.isString());
	BOOST_CHECK_EQUAL(json.asString(), "json");
	BOOST_CHECK(!json.empty());
	BOOST_CHECK_EQUAL(json.size(), 4);
}

BOOST_AUTO_TEST_CASE(make_string_from_std_string)
{
	az::json::Value json(std::string("std"));
	BOOST_CHECK(json.isString());
	BOOST_CHECK_EQUAL(json.asString(), "std");
	BOOST_CHECK(!json.empty());
	BOOST_CHECK_EQUAL(json.size(), 3);
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

BOOST_AUTO_TEST_CASE(make_array_from_one_element_initlist)
{
	az::json::Value json({std::string("json")});
	BOOST_REQUIRE_EQUAL(json.getType(), az::json::Value::Type::Array);
	BOOST_REQUIRE_EQUAL(json.size(), 1);
	BOOST_CHECK_EQUAL(json[0].asString(), "json");
}

BOOST_AUTO_TEST_CASE(make_array_from_json_range)
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

BOOST_AUTO_TEST_CASE(make_array_from_std_container)
{
	std::list<int> list = {1, 22, 333};
	az::json::Value json(list.begin(), list.end());
	BOOST_REQUIRE(json.isArray());
	BOOST_REQUIRE_EQUAL(json.size(), 3);
	BOOST_CHECK_EQUAL(json[0].asInteger(), 1);
	BOOST_CHECK_EQUAL(json[1].asInteger(), 22);
	BOOST_CHECK_EQUAL(json[2].asInteger(), 333);
}

BOOST_AUTO_TEST_CASE(make_array_automatically)
{
	az::json::Value json;
	json[3] = "third";
	BOOST_REQUIRE(json.isArray());
	BOOST_CHECK_EQUAL(json.size(), 4);
	BOOST_CHECK(json.has(3));
}

BOOST_AUTO_TEST_CASE(append_one_to_array)
{
	az::json::Value json = {1};
	BOOST_REQUIRE_EQUAL(json.size(), 1);
	json.append("two");
	BOOST_REQUIRE_EQUAL(json.size(), 2);
	BOOST_CHECK_EQUAL(json[1].asString(), "two");
}

BOOST_AUTO_TEST_CASE(append_one_to_not_array)
{
	az::json::Value json = 123;
	json.append(321);
	BOOST_REQUIRE_EQUAL(json.size(), 1);
	BOOST_CHECK_EQUAL(json[0].asInteger(), 321);
}

BOOST_AUTO_TEST_CASE(append_several_to_array)
{
	az::json::Value json = {nullptr};
	json.append({1, 2.0, "3"});
	BOOST_REQUIRE_EQUAL(json.getType(), az::json::Value::Type::Array);
	BOOST_REQUIRE_EQUAL(json.size(), 4);
	BOOST_CHECK_EQUAL(json[0], az::json::Value(nullptr));
	BOOST_CHECK_EQUAL(json[3], az::json::Value("3"));
}

BOOST_AUTO_TEST_CASE(append_several_to_not_array)
{
	az::json::Value json = true;
	json.append({1, 2.0, "3"});
	BOOST_REQUIRE_EQUAL(json.getType(), az::json::Value::Type::Array);
	BOOST_REQUIRE_EQUAL(json.size(), 3);
	BOOST_CHECK_EQUAL(json[0], az::json::Value(1));
	BOOST_CHECK_EQUAL(json[2], az::json::Value("3"));
}

BOOST_AUTO_TEST_CASE(make_object_from_empty_initlist)
{
	az::json::Value json({});
	BOOST_REQUIRE_EQUAL(json.getType(), az::json::Value::Type::Object);
	BOOST_CHECK_EQUAL(json.size(), 0);
	BOOST_CHECK(json.empty());
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

BOOST_AUTO_TEST_CASE(get_default_by_key)
{
	az::json::Value json = {
		{"yes", 123}
	};
	BOOST_CHECK_EQUAL(json.get("yes", 321).asInteger(), 123);
	BOOST_CHECK_EQUAL(json.get("no", 321).asInteger(), 321);
}

BOOST_AUTO_TEST_CASE(get_default_by_index)
{
	az::json::Value json = {
		0, 1.0, "two"
	};
	BOOST_CHECK_EQUAL(json.get(1, 3.0).asReal(), 1.0);
	BOOST_CHECK_EQUAL(json.get(3, 3.0).asReal(), 3.0);
}

BOOST_AUTO_TEST_CASE(is_equal)
{
	BOOST_CHECK_EQUAL(az::json::Value(), az::json::Value(nullptr));
	BOOST_CHECK_EQUAL(az::json::Value(true), az::json::Value(true));
	BOOST_CHECK_EQUAL(az::json::Value(false), az::json::Value(false));
	BOOST_CHECK_EQUAL(az::json::Value(123), az::json::Value(123));
	BOOST_CHECK_EQUAL(az::json::Value(-456), az::json::Value(-456));
	BOOST_CHECK_EQUAL(az::json::Value(3.14), az::json::Value(3.14));
	BOOST_CHECK_EQUAL(az::json::Value(-2.78), az::json::Value(-2.78));
	BOOST_CHECK_EQUAL(az::json::Value("az"), az::json::Value("az"));
	BOOST_CHECK_EQUAL(az::json::Value({1, 2.0, "three"}), az::json::Value({1, 2.0, "three"}));
	BOOST_CHECK_EQUAL(az::json::Value({{"one", 1}}), az::json::Value({{"one", 1}}));
}

BOOST_AUTO_TEST_CASE(is_null_not_equal)
{
	BOOST_CHECK_NE(az::json::Value(), az::json::Value(true));
	BOOST_CHECK_NE(az::json::Value(), az::json::Value(false));
	BOOST_CHECK_NE(az::json::Value(), az::json::Value(0));
	BOOST_CHECK_NE(az::json::Value(), az::json::Value(-123));
	BOOST_CHECK_NE(az::json::Value(), az::json::Value(3.14));
	BOOST_CHECK_NE(az::json::Value(), az::json::Value(-3.14));
	BOOST_CHECK_NE(az::json::Value(), az::json::Value("az"));
	BOOST_CHECK_NE(az::json::Value(), az::json::Value({1, 2}));
	BOOST_CHECK_NE(az::json::Value(), az::json::Value({{"one", 1}}));
}

BOOST_AUTO_TEST_CASE(is_bool_not_equal)
{
	BOOST_CHECK_NE(az::json::Value(true), az::json::Value());
	BOOST_CHECK_NE(az::json::Value(true), az::json::Value(false));
	BOOST_CHECK_NE(az::json::Value(true), az::json::Value(1));
	BOOST_CHECK_NE(az::json::Value(true), az::json::Value(1.0));
	BOOST_CHECK_NE(az::json::Value(true), az::json::Value("1"));
	BOOST_CHECK_NE(az::json::Value(true), az::json::Value({1, 2}));
	BOOST_CHECK_NE(az::json::Value(true), az::json::Value({{"one", 1}}));

	BOOST_CHECK_NE(az::json::Value(false), az::json::Value());
	BOOST_CHECK_NE(az::json::Value(false), az::json::Value(true));
	BOOST_CHECK_NE(az::json::Value(false), az::json::Value(0));
	BOOST_CHECK_NE(az::json::Value(false), az::json::Value(0.0));
	BOOST_CHECK_NE(az::json::Value(false), az::json::Value("0"));
	BOOST_CHECK_NE(az::json::Value(false), az::json::Value({0, 0}));
	BOOST_CHECK_NE(az::json::Value(false), az::json::Value({{"zero", 0}}));
}

BOOST_AUTO_TEST_CASE(is_integer_not_equal)
{
	BOOST_CHECK_NE(az::json::Value(123), az::json::Value());
	BOOST_CHECK_NE(az::json::Value(123), az::json::Value(true));
	BOOST_CHECK_NE(az::json::Value(123), az::json::Value(false));
	BOOST_CHECK_NE(az::json::Value(123), az::json::Value(0));
	BOOST_CHECK_NE(az::json::Value(123), az::json::Value(-123));
	BOOST_CHECK_NE(az::json::Value(123), az::json::Value(123.0));
	BOOST_CHECK_NE(az::json::Value(123), az::json::Value("123"));
	BOOST_CHECK_NE(az::json::Value(123), az::json::Value({1, 2, 3}));
	BOOST_CHECK_NE(az::json::Value(123), az::json::Value({{"id", 123}}));
}

BOOST_AUTO_TEST_CASE(is_real_not_equal)
{
	BOOST_CHECK_NE(az::json::Value(12.3), az::json::Value());
	BOOST_CHECK_NE(az::json::Value(12.3), az::json::Value(true));
	BOOST_CHECK_NE(az::json::Value(12.3), az::json::Value(false));
	BOOST_CHECK_NE(az::json::Value(12.3), az::json::Value(12));
	BOOST_CHECK_NE(az::json::Value(12.3), az::json::Value(-12.3));
	BOOST_CHECK_NE(az::json::Value(12.3), az::json::Value(12.2999));
	BOOST_CHECK_NE(az::json::Value(12.3), az::json::Value("12.3"));
	BOOST_CHECK_NE(az::json::Value(12.3), az::json::Value({1, 2, 3}));
	BOOST_CHECK_NE(az::json::Value(12.3), az::json::Value({{"id", 123}}));
}

BOOST_AUTO_TEST_CASE(is_string_not_equal)
{
	BOOST_CHECK_NE(az::json::Value("json"), az::json::Value());
	BOOST_CHECK_NE(az::json::Value("json"), az::json::Value(true));
	BOOST_CHECK_NE(az::json::Value("json"), az::json::Value(false));
	BOOST_CHECK_NE(az::json::Value("json"), az::json::Value(0));
	BOOST_CHECK_NE(az::json::Value("json"), az::json::Value(-1));
	BOOST_CHECK_NE(az::json::Value("json"), az::json::Value(1.0));
	BOOST_CHECK_NE(az::json::Value("json"), az::json::Value(-1.0));
	BOOST_CHECK_NE(az::json::Value("json"), az::json::Value({"json", "json"}));
	BOOST_CHECK_NE(az::json::Value("json"), az::json::Value({{"json", 5}}));
}

BOOST_AUTO_TEST_CASE(is_array_not_equal)
{
	az::json::Value json = {1, 2};
	BOOST_REQUIRE(json.isArray());
	BOOST_CHECK_NE(json, az::json::Value());
	BOOST_CHECK_NE(json, az::json::Value(true));
	BOOST_CHECK_NE(json, az::json::Value(false));
	BOOST_CHECK_NE(json, az::json::Value(12345));
	BOOST_CHECK_NE(json, az::json::Value(12.34));
	BOOST_CHECK_NE(json, az::json::Value({1, 2, 3}));
	BOOST_CHECK_NE(json, az::json::Value({{"json", 5}}));
}

BOOST_AUTO_TEST_CASE(is_object_not_equal)
{
	az::json::Value json = {
		{"json", 5}
	};
	BOOST_REQUIRE(json.isObject());
	BOOST_CHECK_NE(json, az::json::Value());
	BOOST_CHECK_NE(json, az::json::Value(true));
	BOOST_CHECK_NE(json, az::json::Value(false));
	BOOST_CHECK_NE(json, az::json::Value(12345));
	BOOST_CHECK_NE(json, az::json::Value(12.34));
	BOOST_CHECK_NE(json, az::json::Value({1, 2, 3}));
	BOOST_CHECK_NE(json, az::json::Value({{"json", 0}}));
}

BOOST_AUTO_TEST_CASE(is_greater_than)
{
	BOOST_CHECK_GT(az::json::Value(true), az::json::Value(false));
	BOOST_CHECK_GT(az::json::Value(123), az::json::Value(122));
	BOOST_CHECK_GT(az::json::Value(3.14), az::json::Value(3.0));
	BOOST_CHECK_GT(az::json::Value("abc"), az::json::Value("abb"));
	BOOST_CHECK_GT(az::json::Value({1, 2, 3}), az::json::Value({1, 2}));
	BOOST_CHECK_GT(az::json::Value({1, 2, 3}), az::json::Value({1, 2, 2}));
	BOOST_CHECK_GT(az::json::Value({{"id", 124}}), az::json::Value({{"id", 123}}));
}

BOOST_AUTO_TEST_CASE(is_greater_or_equal)
{
	BOOST_CHECK_GE(az::json::Value(true), az::json::Value(false));
	BOOST_CHECK_GE(az::json::Value(false), az::json::Value(false));

	BOOST_CHECK_GE(az::json::Value(200), az::json::Value(200));
	BOOST_CHECK_GE(az::json::Value(200), az::json::Value(199));

	BOOST_CHECK_GE(az::json::Value(3.14), az::json::Value(3.13));
	BOOST_CHECK_GE(az::json::Value(3.14), az::json::Value(3.14));

	BOOST_CHECK_GE(az::json::Value("bbb"), az::json::Value("aaa"));
	BOOST_CHECK_GE(az::json::Value("bbb"), az::json::Value("bbb"));

	BOOST_CHECK_GE(az::json::Value({1, 2, 3}), az::json::Value({1, 2}));
	BOOST_CHECK_GE(az::json::Value({1, 2, 3}), az::json::Value({1, 2, 3}));

	BOOST_CHECK_GE(az::json::Value({{"id", 124}}), az::json::Value({{"id", 123}}));
	BOOST_CHECK_GE(az::json::Value({{"id", 124}}), az::json::Value({{"id", 124}}));
}

BOOST_AUTO_TEST_CASE(is_less_than)
{
	BOOST_CHECK_LT(az::json::Value(false), az::json::Value(true));
	BOOST_CHECK_LT(az::json::Value(122), az::json::Value(123));
	BOOST_CHECK_LT(az::json::Value(3.1), az::json::Value(3.2));
	BOOST_CHECK_LT(az::json::Value("aa"), az::json::Value("ab"));
	BOOST_CHECK_LT(az::json::Value({1, 2}), az::json::Value({1, 2, 3}));
	BOOST_CHECK_LT(az::json::Value({1, 2, 3}), az::json::Value({1, 2, 4}));
	BOOST_CHECK_LT(az::json::Value({{"id", 123}}), az::json::Value({{"id", 124}}));
}

BOOST_AUTO_TEST_CASE(is_less_or_equal)
{
	BOOST_CHECK_LE(az::json::Value(false), az::json::Value(true));
	BOOST_CHECK_LE(az::json::Value(false), az::json::Value(false));

	BOOST_CHECK_LE(az::json::Value(122), az::json::Value(123));
	BOOST_CHECK_LE(az::json::Value(123), az::json::Value(123));

	BOOST_CHECK_LE(az::json::Value(3.1), az::json::Value(3.2));
	BOOST_CHECK_LE(az::json::Value(3.2), az::json::Value(3.2));

	BOOST_CHECK_LE(az::json::Value("aa"), az::json::Value("ab"));
	BOOST_CHECK_LE(az::json::Value("ab"), az::json::Value("ab"));

	BOOST_CHECK_LE(az::json::Value({1, 2}), az::json::Value({1, 2, 3}));
	BOOST_CHECK_LE(az::json::Value({1, 2, 4}), az::json::Value({1, 2, 4}));

	BOOST_CHECK_LE(az::json::Value({{"id", 123}}), az::json::Value({{"id", 124}}));
	BOOST_CHECK_LE(az::json::Value({{"id", 124}}), az::json::Value({{"id", 124}}));
}

BOOST_AUTO_TEST_CASE(convert_to_bool)
{
	std::list<az::json::Value> given_values = {
		nullptr, -17, 0, 2020, -3.14, 0.0, 2.78, "true", "false", "True", "False", "any", ""
	};
	for (auto& value : given_values) {
		value.convert(az::json::Value::Type::Bool);
	}
	std::list<az::json::Value> expected_values = {
		false, true, false, true, true, false, true, true, false, true, false, true, false
	};
	BOOST_CHECK_EQUAL_COLLECTIONS(given_values.begin(), given_values.end(), expected_values.begin(), expected_values.end());
}

BOOST_AUTO_TEST_CASE(convert_to_integer)
{
	std::list<az::json::Value> given_values = {
		nullptr, true, false, -3.14, 0.0, 2.78, "-123", "0", "32", "3D", "any"
	};
	for (auto& value : given_values) {
		value.convert(az::json::Value::Type::Integer);
	}
	std::list<az::json::Value> expected_values = {
		0, 1, 0, -3, 0, 2, -123, 0, 32, 3, 0
	};
	BOOST_CHECK_EQUAL_COLLECTIONS(given_values.begin(), given_values.end(), expected_values.begin(), expected_values.end());
}

BOOST_AUTO_TEST_CASE(convert_to_real)
{
	std::list<az::json::Value> given_values = {
		nullptr, true, false, -2020, 0, 2020, "-3.14", "0.0", "2.78", "2.4 GHz", "-9C", "lOSO"
	};
	for (auto& value : given_values) {
		value.convert(az::json::Value::Type::Real);
	}
	std::list<az::json::Value> expected_values = {
		0.0, 1.0, 0.0, -2020.0, 0.0, 2020.0, -3.14, 0.0, 2.78, 2.4, -9.0, 0.0
	};
	BOOST_CHECK_EQUAL_COLLECTIONS(given_values.begin(), given_values.end(), expected_values.begin(), expected_values.end());
}

BOOST_AUTO_TEST_CASE(convert_to_string)
{
	std::list<az::json::Value> given_values = {
		nullptr, true, false, -2020, 0, 2020, -3.14, 0.0, 2.78
	};
	for (auto& value : given_values) {
		value.convert(az::json::Value::Type::String);
	}
	std::list<az::json::Value> expected_values = {
		"null", "true", "false", "-2020", "0", "2020", "-3.14", "0", "2.78"
	};
	BOOST_CHECK_EQUAL_COLLECTIONS(given_values.begin(), given_values.end(), expected_values.begin(), expected_values.end());
}

BOOST_AUTO_TEST_CASE(convert_object_to_array)
{
	az::json::Value json = {
		{"bool", true},
		{"integer", 123},
		{"real", 3.14},
		{"string", "pizza"}
	};
	json.convert(az::json::Value::Type::Array);
	BOOST_REQUIRE(json.isArray());
	BOOST_REQUIRE_EQUAL(json.size(), 4);
	BOOST_CHECK_EQUAL(json[0], az::json::Value(true));
	BOOST_CHECK_EQUAL(json[1], az::json::Value(123));
	BOOST_CHECK_EQUAL(json[2], az::json::Value(3.14));
	BOOST_CHECK_EQUAL(json[3], az::json::Value("pizza"));
}

BOOST_AUTO_TEST_CASE(convert_not_object_to_array)
{
	az::json::Value json("wow");
	json.convert(az::json::Value::Type::Array);
	BOOST_REQUIRE(json.isArray());
	BOOST_REQUIRE_EQUAL(json.size(), 1);
	BOOST_REQUIRE(json.has(0));
	BOOST_CHECK_EQUAL(json[0], az::json::Value("wow"));
}

BOOST_AUTO_TEST_CASE(convert_array_to_object)
{
	az::json::Value json = {
		nullptr, true, 123, 3.14
	};
	json.convert(az::json::Value::Type::Object);
	BOOST_REQUIRE(json.isObject());
	BOOST_REQUIRE_EQUAL(json.size(), 4);
	BOOST_CHECK_EQUAL(json["id0"], az::json::Value(nullptr));
	BOOST_CHECK_EQUAL(json["id1"], az::json::Value(true));
	BOOST_CHECK_EQUAL(json["id2"], az::json::Value(123));
	BOOST_CHECK_EQUAL(json["id3"], az::json::Value(3.14));
}

BOOST_AUTO_TEST_CASE(convert_not_array_to_object)
{
	az::json::Value json(123);
	json.convert(az::json::Value::Type::Object);

	BOOST_REQUIRE(json.isObject());
	BOOST_REQUIRE_EQUAL(json.size(), 1);
	BOOST_REQUIRE(json.has("integer"));
	BOOST_CHECK_EQUAL(json["integer"], az::json::Value(123));
}

BOOST_AUTO_TEST_CASE(iterate_array)
{
	az::json::Value given = {
		nullptr, true, 123, 3.14, "json"
	};
	std::list<az::json::Value> expected = {
		nullptr, true, 123, 3.14, "json"
	};
	BOOST_CHECK_EQUAL_COLLECTIONS(given.begin(), given.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(iterate_object)
{
	az::json::Value json = {
		{"bool", false},
		{"integer", 123},
		{"null", nullptr},
		{"real", 2.78},
		{"string", "coffee"}
	};

	std::list<std::string> keys;
	std::list<az::json::Value> values;
	for (auto it = json.begin(); it != json.end(); it++) {
		keys.push_back(it.key());
		values.push_back(it.value());
	}

	std::list<std::string> expected_keys = {
		"bool", "integer", "null", "real", "string"
	};
	std::list<az::json::Value> expected_values = {
		false, 123, nullptr, 2.78, "coffee"
	};
	BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(), expected_keys.begin(), expected_keys.end());
	BOOST_CHECK_EQUAL_COLLECTIONS(values.begin(), values.end(), expected_values.begin(), expected_values.end());
}

BOOST_AUTO_TEST_CASE(iterate_null)
{
	az::json::Value json;
	uint32_t counter = 0;
	for (auto it = json.begin(); it != json.end(); it++) {
		counter++;
	}
	BOOST_CHECK_EQUAL(counter, 0);
}

BOOST_AUTO_TEST_SUITE_END()
