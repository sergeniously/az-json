#include <boost/test/unit_test.hpp>
#include "Path.h"

BOOST_AUTO_TEST_SUITE(PathTests)

BOOST_AUTO_TEST_CASE(resolve_root)
{
	az::json::Value json = az::json::Value::Type::Object;
	BOOST_CHECK_EQUAL(az::json::Path(".").resolve(json).getType(), az::json::Value::Type::Object);
}

BOOST_AUTO_TEST_CASE(resolve_empty)
{
	az::json::Value json = az::json::Value::Type::Array;
	BOOST_REQUIRE_NO_THROW(az::json::Path("").resolve(json));
	BOOST_CHECK_EQUAL(az::json::Path("").resolve(json).getType(), az::json::Value::Type::Null);
}

BOOST_AUTO_TEST_CASE(resolve_object)
{
	az::json::Value json = {
		{"id", 0xC0FFEE},
		{"not id", "json5"},
		{"array", {
			true,
			1.2,
			{
				{"one", 1},
				{"two", 2}
			}
		}},
		{"object", {
			{"number", 123},
			{"array", {1,2}}
		}},
	};
	BOOST_CHECK_EQUAL(az::json::Path(".id").resolve(json).getType(), az::json::Value::Type::Integer);
	BOOST_CHECK_EQUAL(az::json::Path(".id").resolve(json).asInteger(), 0xc0ffee);

	BOOST_CHECK_EQUAL(az::json::Path(".'not id'").resolve(json).getType(), az::json::Value::Type::String);
	BOOST_CHECK_EQUAL(az::json::Path(".'not id'").resolve(json).asString(), "json5");

	BOOST_CHECK_EQUAL(az::json::Path(".array").resolve(json).getType(), az::json::Value::Type::Array);
	BOOST_CHECK_EQUAL(az::json::Path(".array").resolve(json).size(), 3);

	BOOST_CHECK_EQUAL(az::json::Path(".array[0]").resolve(json).getType(), az::json::Value::Type::Bool);
	BOOST_CHECK_EQUAL(az::json::Path(".array[0]").resolve(json).asBool(), true);

	BOOST_CHECK_EQUAL(az::json::Path(".array[1]").resolve(json).getType(), az::json::Value::Type::Real);
	BOOST_CHECK_EQUAL(az::json::Path(".array[1]").resolve(json).asReal(), 1.2);

	BOOST_CHECK_EQUAL(az::json::Path(".array[2]").resolve(json).getType(), az::json::Value::Type::Object);
	BOOST_CHECK_EQUAL(az::json::Path(".array[2]").resolve(json).size(), 2);

	BOOST_CHECK_EQUAL(az::json::Path(".array[2].one").resolve(json).getType(), az::json::Value::Type::Integer);
	BOOST_CHECK_EQUAL(az::json::Path(".array[2].one").resolve(json).asInteger(), 1);
	BOOST_CHECK_EQUAL(az::json::Path(".array[2].two").resolve(json).getType(), az::json::Value::Type::Integer);
	BOOST_CHECK_EQUAL(az::json::Path(".array[2].two").resolve(json).asInteger(), 2);

	BOOST_CHECK_EQUAL(az::json::Path(".object").resolve(json).getType(), az::json::Value::Type::Object);
	BOOST_CHECK_EQUAL(az::json::Path(".object").resolve(json).size(), 2);

	BOOST_CHECK_EQUAL(az::json::Path(".object.number").resolve(json).getType(), az::json::Value::Type::Integer);
	BOOST_CHECK_EQUAL(az::json::Path(".object.number").resolve(json).asInteger(), 123);

	BOOST_CHECK_EQUAL(az::json::Path(".object.array").resolve(json).getType(), az::json::Value::Type::Array);
	BOOST_CHECK_EQUAL(az::json::Path(".object.array").resolve(json).size(), 2);

	BOOST_CHECK_EQUAL(az::json::Path(".object.array[1]").resolve(json).getType(), az::json::Value::Type::Integer);
	BOOST_CHECK_EQUAL(az::json::Path(".object.array[1]").resolve(json).asInteger(), 2);

	BOOST_CHECK_EQUAL(az::json::Path(".idk.wtf").resolve(json).getType(), az::json::Value::Type::Null);
}

BOOST_AUTO_TEST_CASE(resolve_array)
{
	az::json::Value json = {
		1, 2.0, "three", {true, {1,2,3}}
	};

	BOOST_CHECK_EQUAL(az::json::Path("[2]").resolve(json).getType(), az::json::Value::Type::String);
	BOOST_CHECK_EQUAL(az::json::Path("[2]").resolve(json).asString(), "three");

	BOOST_CHECK_EQUAL(az::json::Path("[3][0]").resolve(json).getType(), az::json::Value::Type::Bool);
	BOOST_CHECK_EQUAL(az::json::Path("[3][0]").resolve(json).asBool(), true);

	BOOST_CHECK_EQUAL(az::json::Path("[3][1][1]").resolve(json).getType(), az::json::Value::Type::Integer);
	BOOST_CHECK_EQUAL(az::json::Path("[3][1][1]").resolve(json).asInteger(), 2);

	BOOST_CHECK_EQUAL(az::json::Path("[3][3]").resolve(json).getType(), az::json::Value::Type::Null);
}

BOOST_AUTO_TEST_CASE(make_object)
{
	az::json::Value json;
	az::json::Path(".one").make(json) = 1;
	BOOST_REQUIRE(json.has("one"));
	BOOST_CHECK_EQUAL(json["one"].getType(), az::json::Value::Type::Integer);
	BOOST_CHECK_EQUAL(json["one"].asInteger(), 1);
}

BOOST_AUTO_TEST_CASE(make_objects)
{
	az::json::Value json;
	az::json::Path(".one.two.three").make(json) = {1,2,3};
	BOOST_REQUIRE(json.has("one"));
	BOOST_REQUIRE(json["one"].has("two"));
	BOOST_REQUIRE(json["one"]["two"].has("three"));
	BOOST_CHECK_EQUAL(json["one"]["two"]["three"].getType(), az::json::Value::Type::Array);
}

BOOST_AUTO_TEST_CASE(make_array)
{
	az::json::Value json;
	az::json::Path("[0]").make(json) = 1.0;
	BOOST_REQUIRE(json.has(0));
	BOOST_CHECK_EQUAL(json[0].getType(), az::json::Value::Type::Real);
	BOOST_CHECK_EQUAL(json[0].asReal(), 1);
}

BOOST_AUTO_TEST_CASE(make_arrays)
{
	az::json::Value json;
	az::json::Path("[0][1][2]").make(json) = az::json::Value::Object();
	BOOST_REQUIRE(json.has(0));
	BOOST_REQUIRE_EQUAL(json.size(), 1);

	BOOST_REQUIRE(json[0].has(1));
	BOOST_REQUIRE_EQUAL(json[0].size(), 2);

	BOOST_REQUIRE(json[0][1].has(2));
	BOOST_REQUIRE_EQUAL(json[0][1].size(), 3);

	BOOST_CHECK_EQUAL(json[0][1][2].getType(), az::json::Value::Type::Object);
}

BOOST_AUTO_TEST_CASE(make_complex)
{
	az::json::Value json;
	az::json::Path(".array[0].'127.0.0.1'").make(json) = "localhost";
	BOOST_REQUIRE_EQUAL(json["array"][0]["127.0.0.1"].getType(), az::json::Value::Type::String);
	BOOST_CHECK_EQUAL(json["array"][0]["127.0.0.1"].asString(), "localhost");
}

BOOST_AUTO_TEST_SUITE_END()
