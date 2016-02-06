
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

#define BOOST_TEST_MODULE GrammarTestModule

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <string>
#include "grammar.h"

// The two relevant Boost namespaces for the unit test framework are:
using namespace boost;
using namespace boost::unit_test;

// Provide a name for our suite of tests. This statement is used to bracket
// our test cases.
BOOST_AUTO_TEST_SUITE(GrammarTestSuite)

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
    
    std::ostringstream os;

};

// Define a test case. The first argument specifies the name of the test.
// Take some care in naming your tests. Do not reuse names or accidentally use
// the same name for a test as specified for the module test suite name.
//
// The second argument provides a test build-up/tear-down object that is
// responsible for creating and destroying any resources needed by the
// unit test
BOOST_FIXTURE_TEST_CASE(Grammar_Fatal_1, TestFixture)
{
    Grammar G("fatal1.grammar");
    //printf("status: %d\n", G.status());
    BOOST_CHECK(G.status() == Grammar::CHECK_FATAL);
    //printf("'%s'\n", G.issues().c_str());

    std::string expect =
        "Missing rule: ADDRESS : referenced at []\n"
        "Rule 'A' defined by not referenced!\n"
        "Rule 'AB' defined by not referenced!\n"
        "Rule 'BC' defined by not referenced!\n"
        "Rule 'CD' defined by not referenced!\n"
        "Rule 'DE' defined by not referenced!\n"
        "Rule 'EF' defined by not referenced!\n"
        "Rule 'ROOT' defined by not referenced!\n";

    BOOST_CHECK(G.issues() == expect);
}

BOOST_FIXTURE_TEST_CASE(Grammar_Fatal_2, TestFixture)
{
    Grammar G("fatal2.grammar");
    //printf("status: %d\n", G.status());
    BOOST_CHECK(G.status() == Grammar::CHECK_FATAL);
    //printf("'%s'\n", G.issues().c_str());

    std::string expect =
        "Score <= 0 at [AB]\n"
        "Score <= 0 at [BC]\n"
        "Missing rule: XY : referenced at [ADDRESS]\n"
        "Rule 'XYZ' defined by not referenced!\n";

    BOOST_CHECK(G.issues() == expect);
}

BOOST_FIXTURE_TEST_CASE(Grammar_Warn, TestFixture)
{
    Grammar G("warn.grammar");
    //printf("status: %d\n", G.status());
    BOOST_CHECK(G.status() == Grammar::CHECK_WARN);
    //printf("'%s'\n", G.issues().c_str());

    std::string expect =
        "Score <= 0 at [A]\n"
        "Rule 'XY' defined by not referenced!\n";

    BOOST_CHECK(G.issues() == expect);
}

BOOST_FIXTURE_TEST_CASE(Grammar_Good, TestFixture)
{
    Grammar G("good.grammar");
    //printf("status: %d\n", G.status());
    BOOST_CHECK(G.status() == Grammar::CHECK_OK);
    //printf("'%s'\n", G.issues().c_str());
    BOOST_CHECK(G.issues() == "");

    os.str("");
    os << G;

    std::string expect = 
        "[A]\n"
        "NUMBER -> BLDNG -> 0.5\n\n"
        "[AB]\n"
        "NUMBER WORD -> BLDNG HOUSE -> 0.5\n\n"
        "[ADDRESS]\n"
        "@AB @CD @EF\n"
        "@A @BC @DE\n"
        "@CD @EF\n\n"
        "[BC]\n"
        "WORD TYPE -> HOUSE PREDIR -> 0.5\n\n"
        "[CD]\n"
        "TYPE QUALIF -> PREDIR QUALIF -> 0.5\n\n"
        "[DE]\n"
        "QUALIF ROAD -> QUALIF SUFTYP -> 0.5\n\n"
        "[EF]\n"
        "ROAD RR -> SUFTYP SUFDIR -> 0.5\n\n";

    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == expect);
}

// This must match the BOOST_AUTO_TEST_SUITE(ExampleTestSuite) statement
// above and is used to bracket our test cases.

BOOST_AUTO_TEST_SUITE_END()

