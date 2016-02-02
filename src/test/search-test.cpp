
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
#include "search.h"

// The two relevant Boost namespaces for the unit test framework are:
using namespace boost;
using namespace boost::unit_test;

// Provide a name for our suite of tests. This statement is used to bracket
// our test cases.
BOOST_AUTO_TEST_SUITE(SearchTestSuite)

// The structure below allows us to pass a test initialization object to
// each test case. Note the use of struct to default all methods and member
// variables to public access.
struct TestFixture
{
    TestFixture() :
        G("good.grammar")
    {
        // Put test initialization here, the constructor will be called
        // prior to the execution of each test case
        //printf("Initialize test\n");

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

        os.str(""); // clear
        os << G;

        //printf("'%s'\n", os.str().c_str());
        BOOST_CHECK(os.str() == expect);
        //printf("status: %d\n", G.status());
        BOOST_CHECK(G.status() == Grammar::CHECK_OK);
        //printf("'%s'\n", G.issues().c_str());
        BOOST_CHECK(G.issues() == "");

        os.str(""); // clear
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
    Grammar G;

    std::string resultAsString(const std::vector<Rule> &result) {
        std::ostringstream ss;
        for (const auto &r : result)
            ss << r << "\n";

        return ss.str();
    }

};

// Define a test case. The first argument specifies the name of the test.
// Take some care in naming your tests. Do not reuse names or accidentally use
// the same name for a test as specified for the module test suite name.
//
// The second argument provides a test build-up/tear-down object that is
// responsible for creating and destroying any resources needed by the
// unit test
BOOST_FIXTURE_TEST_CASE(SearchTest_1, TestFixture)
{
    // a pattern we should match
    std::vector<InClass::Type> pattern;
    pattern.push_back(InClass::NUMBER);
    pattern.push_back(InClass::WORD);
    pattern.push_back(InClass::TYPE);
    pattern.push_back(InClass::QUALIF);
    pattern.push_back(InClass::ROAD);

    Search s(G, pattern);
    s.search();
    int num = s.numResults();
    BOOST_CHECK(num == 1);

    std::string expect =
        "NUMBER -> BLDNG -> 0.5\n"
        "WORD TYPE -> HOUSE PREDIR -> 0.5\n"
        "QUALIF ROAD -> QUALIF SUFTYP -> 0.5\n";

    std::vector<std::vector<Rule> > results = s.results();
    os.str(""); // clear
    for (const auto &e : results) {
        os << resultAsString( e );
    }
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == expect);

    std::vector<Rule> best = s.bestResult();
    os.str(""); // clear
    os << resultAsString( best );
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == expect);

}

BOOST_FIXTURE_TEST_CASE(SearchTest_2, TestFixture)
{
    // a pattern we should match
    std::vector<InClass::Type> pattern;
    pattern.push_back(InClass::NUMBER);
    pattern.push_back(InClass::WORD);
    pattern.push_back(InClass::TYPE);
    pattern.push_back(InClass::QUALIF);
    pattern.push_back(InClass::ROAD);

    Search s(G);
    s.search(pattern);
    int num = s.numResults();
    BOOST_CHECK(num == 1);

    std::string expect =
        "NUMBER -> BLDNG -> 0.5\n"
        "WORD TYPE -> HOUSE PREDIR -> 0.5\n"
        "QUALIF ROAD -> QUALIF SUFTYP -> 0.5\n";

    std::vector<std::vector<Rule> > results = s.results();
    os.str(""); // clear
    for (const auto &e : results) {
        os << resultAsString( e );
    }
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == expect);

    std::vector<Rule> best = s.bestResult();
    os.str(""); // clear
    os << resultAsString( best );
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == expect);

}

BOOST_FIXTURE_TEST_CASE(SearchTest_3, TestFixture)
{
    // a pattern we should match
    std::vector<InClass::Type> pattern;
    pattern.push_back(InClass::NUMBER);
    pattern.push_back(InClass::WORD);
    pattern.push_back(InClass::TYPE);
    pattern.push_back(InClass::QUALIF);
    pattern.push_back(InClass::ROAD);
    pattern.push_back(InClass::RR);

    Search s(G);
    s.search(pattern);
    int num = s.numResults();
    BOOST_CHECK(num == 1);

    std::string expect =
        "NUMBER WORD -> BLDNG HOUSE -> 0.5\n"
        "TYPE QUALIF -> PREDIR QUALIF -> 0.5\n"
        "ROAD RR -> SUFTYP SUFDIR -> 0.5\n";

    std::vector<std::vector<Rule> > results = s.results();
    os.str(""); // clear
    for (const auto &e : results) {
        os << resultAsString( e );
    }
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == expect);

    std::vector<Rule> best = s.bestResult();
    os.str(""); // clear
    os << resultAsString( best );
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == expect);

}

BOOST_FIXTURE_TEST_CASE(SearchTest_4, TestFixture)
{
    // a pattern we should FAIL to match
    std::vector<InClass::Type> pattern;
    pattern.push_back(InClass::NUMBER);
    pattern.push_back(InClass::WORD);
    pattern.push_back(InClass::TYPE);
    pattern.push_back(InClass::ROAD);

    Search s(G);
    s.search(pattern);
    int num = s.numResults();
    BOOST_CHECK(num == 0);

    std::string expect = "";

    std::vector<std::vector<Rule> > results = s.results();
    os.str(""); // clear
    for (const auto &e : results) {
        os << resultAsString( e );
    }
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == expect);

    std::vector<Rule> best = s.bestResult();
    os.str(""); // clear
    os << resultAsString( best );
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == expect);

}

// This must match the BOOST_AUTO_TEST_SUITE(ExampleTestSuite) statement
// above and is used to bracket our test cases.

BOOST_AUTO_TEST_SUITE_END()

