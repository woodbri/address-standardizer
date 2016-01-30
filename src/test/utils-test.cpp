
// The following two defines are required by the Boost unit test framework
// to create the necessary testing support. These defines must be placed
// before the inclusion of the boost headers.
//
// The first define provides a name for our Boost test module.
//
// The second of these defines is used to indicate that we are building a
// unit test module that will link dynamically with Boost. If you are using
// a static library version of Boost, this define must be deleted. (or
// in this case commented out)
//
// and include the test headers

#define BOOST_TEST_MODULE OutClassTestModule

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "utils.h"

// The two relevant Boost namespaces for the unit test framework are:
using namespace boost;
using namespace boost::unit_test;

// Provide a name for our suite of tests. This statement is used to bracket
// our test cases.
BOOST_AUTO_TEST_SUITE(UtilsTestSuite)

// The structure below allows us to pass a test initialization object to
// each test case. Note the use of struct to default all methods and member
// variables to public access.
struct TestFixture
{
    TestFixture() {
        // Put test initialization here, the constructor will be called
        // prior to the execution of each test case
        //printf("Initialize test\n");
    }
    ~TestFixture() {
        // Put test cleanup here, the destructor will automatically be
        // invoked at the end of each test case.
        //printf("Cleanup test\n");
    }
    // Public test fixture variables are automatically available to all test
    // cases. Don’t forget to initialize these variables in the constructors
    // to avoid initialized variable errors.

};

// Define a test case. The first argument specifies the name of the test.
// Take some care in naming your tests. Do not reuse names or accidentally use
// the same name for a test as specified for the module test suite name.
//
// The second argument provides a test build-up/tear-down object that is
// responsible for creating and destroying any resources needed by the
// unit test
BOOST_FIXTURE_TEST_CASE(Utils_unaccentUTF8, TestFixture)
{
    std::string s, t;

    s = "Dürerstraße";
    t = Utils::unaccentUTF8( s );
    //printf("unaccentUTF8: '%s' -> '%s'\n", s.c_str(), t.c_str());
    BOOST_REQUIRE(t == "Durerstraße");

    s = "An der Wöhrde";
    t = Utils::unaccentUTF8( s );
    //printf("unaccentUTF8: '%s' -> '%s'\n", s.c_str(), t.c_str());
    BOOST_REQUIRE(t == "An der Wohrde");

    s = "Engelmannsbäke, Endel";
    t = Utils::unaccentUTF8( s );
    //printf("unaccentUTF8: '%s' -> '%s'\n", s.c_str(), t.c_str());
    BOOST_REQUIRE(t == "Engelmannsbake, Endel");

}

BOOST_FIXTURE_TEST_CASE(Utils_upperCaseUTF8, TestFixture)
{
    std::string s, t;

    s = "Dürerstraße";
    t = Utils::upperCaseUTF8( s, "de_DE" );
    //printf("upperCaseUTF8: '%s' -> '%s'\n", s.c_str(), t.c_str());
    BOOST_REQUIRE(t == "DÜRERSTRASSE");

    s = "An der Wöhrde";
    t = Utils::upperCaseUTF8( s, "de_DE" );
    //printf("upperCaseUTF8: '%s' -> '%s'\n", s.c_str(), t.c_str());
    BOOST_REQUIRE(t == "AN DER WÖHRDE");

    s = "Engelmannsbäke, Endel";
    t = Utils::upperCaseUTF8( s, "de_DE" );
    //printf("upperCaseUTF8: '%s' -> '%s'\n", s.c_str(), t.c_str());
    BOOST_REQUIRE(t == "ENGELMANNSBÄKE, ENDEL");

}

BOOST_FIXTURE_TEST_CASE(Utils_normalizeUTF8, TestFixture)
{
    std::string s, t;
    UErrorCode errorCode;

    s = "Dürerstraße";
    t = Utils::normalizeUTF8( s, errorCode );
    //printf("normalizeUTF8: '%s' -> '%s'\n", s.c_str(), t.c_str());
    BOOST_REQUIRE(t == "Dürerstraße");

    s = "An der Wöhrde";
    t = Utils::normalizeUTF8( s, errorCode );
    //printf("normalizeUTF8: '%s' -> '%s'\n", s.c_str(), t.c_str());
    BOOST_REQUIRE(t == "An der Wöhrde");

    s = "Engelmannsbäke, Endel";
    t = Utils::normalizeUTF8( s, errorCode );
    //printf("normalizeUTF8: '%s' -> '%s'\n", s.c_str(), t.c_str());
    BOOST_REQUIRE(t == "Engelmannsbäke, Endel");
    
}

// This must match the BOOST_AUTO_TEST_SUITE(ExampleTestSuite) statement
// above and is used to bracket our test cases.

BOOST_AUTO_TEST_SUITE_END()

