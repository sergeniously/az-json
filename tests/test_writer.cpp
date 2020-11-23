#include <json/Writer.h>
#include "test.hpp"

BOOST_AUTO_TEST_SUITE(WriterTests)

BOOST_AUTO_TEST_CASE(write_object)
{
	az::json::Value json = {
		{"one", 1},
		{"two", {2,2,2}},
		{"three", {
			{"3rd", 3}
		}}
	};
	az::json::Writer(std::cout).pretty().write(json);
}

BOOST_AUTO_TEST_SUITE_END()
