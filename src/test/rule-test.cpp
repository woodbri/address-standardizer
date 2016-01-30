
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

#include <string>
#include "rule.h"

// The two relevant Boost namespaces for the unit test framework are:
using namespace boost;
using namespace boost::unit_test;

// Provide a name for our suite of tests. This statement is used to bracket
// our test cases.
BOOST_AUTO_TEST_SUITE(RuleTestSuite)

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
BOOST_FIXTURE_TEST_CASE(Rule_Constructors, TestFixture)
{
    std::ostringstream os;

    // Verify default constructor with operator<<
    Rule r1;
    os << r1;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "-> -> 0");
    BOOST_CHECK(r1.isEmpty());

    std::string sr2 = " -> -> 0.0";
    Rule r2(sr2, false);
    os.str(""); // clear os
    os << r2;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "-> -> 0");

    std::string sr3 = "NUMBER WORD TYPE -> HOUSE STREET SUFTYP -> 0.2";
    Rule r3(sr3, false);
    os.str(""); // clear os
    os << r3;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "NUMBER WORD TYPE -> HOUSE STREET SUFTYP -> 0.2");

    Rule r3f(sr3, true);
    os.str(""); // clear os
    os << r3f;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(r3f.isEmpty());
    BOOST_CHECK(os.str() == "-> -> 0");

    std::string sr4 = "NUMBER WORD -> HOUSE STREET SUFTYP -> 0.2";
    Rule r4(sr4, false);
    os.str(""); // clear os
    os << r4;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(r3f.isEmpty());
    BOOST_CHECK(os.str() == "-> -> 0");

    std::string sr5 = " NUMBER   WORD TYPE  ->  HOUSE STREET  SUFTYP -> 0.2";
    Rule r5(sr5, false);
    os.str(""); // clear os
    os << r5;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "NUMBER WORD TYPE -> HOUSE STREET SUFTYP -> 0.2");

    std::string sr6 = "@HOUSE_NUMBER @STREET_NAME @MACRO";
    Rule r6(sr6, true);
    os.str(""); // clear os
    os << r6;
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == "@HOUSE_NUMBER @STREET_NAME @MACRO");

    Rule r6f(sr6, false);
    os.str(""); // clear os
    os << r6f;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(r6f.isEmpty());
    BOOST_CHECK(os.str() == "-> -> 0");

    Rule r7( r5 );
    os.str(""); // clear os
    os << r7;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "NUMBER WORD TYPE -> HOUSE STREET SUFTYP -> 0.2");

    Rule r8( r6 );
    os.str(""); // clear os
    os << r8;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "@HOUSE_NUMBER @STREET_NAME @MACRO");

}

BOOST_FIXTURE_TEST_CASE(Rule_getters, TestFixture)
{
    std::ostringstream os;

    std::string sr = "NUMBER WORD TYPE -> HOUSE STREET SUFTYP -> 0.2";
    Rule r(sr, false);
    os.str(""); // clear os
    os << r;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "NUMBER WORD TYPE -> HOUSE STREET SUFTYP -> 0.2");

    BOOST_CHECK( r.isMeta() == false );
    BOOST_CHECK( r.meta().size() == 0 );
    BOOST_CHECK( r.in().size() == 3 );
    BOOST_CHECK( r.out().size() == 3 );
    BOOST_CHECK_CLOSE( r.score(), 0.2, 0.001 );
    BOOST_CHECK( r.isValid() == true );
    BOOST_CHECK( r.isEmpty() == false );

    std::string sr2 = "@HOUSE_NUMBER @STREET_NAME @MACRO";
    Rule r2(sr2, true);
    os.str(""); // clear os
    os << r2;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "@HOUSE_NUMBER @STREET_NAME @MACRO");

    BOOST_CHECK( r2.isMeta() == true );
    BOOST_CHECK( r2.meta().size() == 3 );
    BOOST_CHECK( r2.in().size() == 0 );
    BOOST_CHECK( r2.out().size() == 0 );
    BOOST_CHECK_CLOSE( r2.score(), 0.0, 0.001 );
    BOOST_CHECK( r2.isValid() == true );
    BOOST_CHECK( r2.isEmpty() == false );

}

BOOST_FIXTURE_TEST_CASE(Rule_mutators_test1, TestFixture)
{
    std::ostringstream os;

    std::string sr = "NUMBER WORD TYPE -> HOUSE STREET SUFTYP -> 0.2";
    Rule r(sr, false);
    os.str(""); // clear os
    os << r;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "NUMBER WORD TYPE -> HOUSE STREET SUFTYP -> 0.2");

    std::string sr1 = "WORD PROV NUMBER -> CITY PROV POSTAL -> 0.4";
    Rule r1(sr1, false);
    os.str(""); // clear os
    os << r1;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "WORD PROV NUMBER -> CITY PROV POSTAL -> 0.4");

    // concat(r)
    Rule cr = r.concat( r1 );
    os.str(""); // clear os
    os << cr;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "NUMBER WORD TYPE WORD PROV NUMBER -> HOUSE STREET SUFTYP CITY PROV POSTAL -> 0.3");

    // opertaor+
    Rule cr2 = r + r1;
    os.str(""); // clear os
    os << cr2;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "NUMBER WORD TYPE WORD PROV NUMBER -> HOUSE STREET SUFTYP CITY PROV POSTAL -> 0.3");

    // clear()
    cr2.clear();
    os.str(""); // clear os
    os << cr2;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "-> -> 0");
    BOOST_CHECK( cr2.isEmpty() );

}

// This must match the BOOST_AUTO_TEST_SUITE(ExampleTestSuite) statement
// above and is used to bracket our test cases.

BOOST_AUTO_TEST_SUITE_END()

