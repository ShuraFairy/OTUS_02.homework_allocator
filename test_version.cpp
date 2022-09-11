#define BOOST_TEST_MODULE test_version

#include "lib.h"
#include "custom_container.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_version)

BOOST_AUTO_TEST_CASE(test_valid_version) 
{
	BOOST_CHECK(version() > 0);
}

BOOST_AUTO_TEST_CASE(test_custom_forward_list_iterator) 
{
    custom_forward_list<int> int_list;
    int_list.push_back(1);
    int_list.push_back(2);
    auto iterator_begin = int_list.begin();
//    auto iterator_end = int_list.end();
    BOOST_REQUIRE_EQUAL(*iterator_begin, 1);
    //BOOST_REQUIRE_EQUAL(*iterator_end, NULL);
}

BOOST_AUTO_TEST_CASE(test_custom_forward_list_case_1) 
{
    custom_forward_list<int> int_list;
    int_list.push_back(3);
    int_list.push_back(2);
    int_list.push_back(1);
    BOOST_REQUIRE_EQUAL(int_list.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_custom_forward_list_case_2)
{
    custom_forward_list<int> int_list;
    BOOST_REQUIRE_EQUAL(int_list.empty(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
