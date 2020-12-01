#define _CRT_SECURE_NO_WARNINGS
#include "../Writer.h"
#include "tests.hpp"
#include <ctime>

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
