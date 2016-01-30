
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
#include "lexentry.h"

// The two relevant Boost namespaces for the unit test framework are:
using namespace boost;
using namespace boost::unit_test;

// Provide a name for our suite of tests. This statement is used to bracket
// our test cases.
BOOST_AUTO_TEST_SUITE(LexEntryTestSuite)

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
BOOST_FIXTURE_TEST_CASE(LexEntry_Constructors, TestFixture)
{
    std::ostringstream os;

    // Verify default constructor with operator<<
    LexEntry e1;
    os << e1;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXENTRY:\t\t\t\t");

    os.str(""); // clear os
    std::string word = "FOOBAR";
    std::string stdword = "FOOBAR";
    std::string stype = "WORD";
    std::string stype2 = "WORD,CITY";
    std::string attach = "DETACH";
    std::string attach2 = "DET_PRE,DET_SUF";

    LexEntry e2(word, stdword, stype, attach);
    os << e2;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXENTRY:\tFOOBAR\tFOOBAR\tWORD\t");

    os.str(""); // clear os
    LexEntry e2a(word, stdword, stype2, attach2);
    os << e2a;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXENTRY:\tFOOBAR\tFOOBAR\tWORD,CITY\tDET_PRE,DET_SUF");

    os.str(""); // clear os
    LexEntry e3("FOOBAR\tFOOBAR\tWORD\tDETACH");
    os << e3;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXENTRY:\tFOOBAR\tFOOBAR\tWORD\t");

    os.str(""); // clear os
    LexEntry e4("LEXENTRY:\tFOOBAR\tFOOBAR\tWORD\tDETACH");
    os << e4;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXENTRY:\tFOOBAR\tFOOBAR\tWORD\t");

    os.str(""); // clear os
    LexEntry e5("LEXENTRY:\tFOOBAR\tFOOBAR\tWORD,CITY\tDETACH");
    os << e5;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXENTRY:\tFOOBAR\tFOOBAR\tWORD,CITY\t");

    os.str(""); // clear os
    LexEntry e6("LEXENTRY:\tFOOBAR\tFOOBAR\tCITY,WORD\tDET_PRE,DET_SUF");
    os << e6;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXENTRY:\tFOOBAR\tFOOBAR\tWORD,CITY\tDET_PRE,DET_SUF");
}

BOOST_FIXTURE_TEST_CASE(LexEntry_getters, TestFixture)
{
    std::ostringstream os;

    LexEntry e("LEXENTRY:\tFOO\tFOOBAR\tWORD,CITY\tDET_PRE,DET_SUF");
    os << e;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXENTRY:\tFOO\tFOOBAR\tWORD,CITY\tDET_PRE,DET_SUF");
    BOOST_CHECK(e.word() == "FOO");
    BOOST_CHECK(e.stdword() == "FOOBAR");
    BOOST_CHECK(InClass::asString(e.type()) == "WORD,CITY");
    BOOST_CHECK(InClass::asString(e.attached()) == "DET_PRE,DET_SUF");
    BOOST_CHECK(e.isPrefix() == true);
    BOOST_CHECK(e.isSuffix() == true);
    BOOST_CHECK(e.isAttached() == false);
    BOOST_CHECK(e.isDetached() == true);
    BOOST_CHECK(e.isPrefixAttached() == false);
    BOOST_CHECK(e.isSuffixAttached() == false);

    os.str(""); // clear
    LexEntry e2("FOO\tFOOBAR\tWORD,CITY\tATT_PRE,DET_SUF");
    os << e2;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXENTRY:\tFOO\tFOOBAR\tWORD,CITY\tDET_SUF,ATT_PRE");
    BOOST_CHECK(e2.word() == "FOO");
    BOOST_CHECK(e2.stdword() == "FOOBAR");
    BOOST_CHECK(InClass::asString(e2.type()) == "WORD,CITY");
    BOOST_CHECK(InClass::asString(e2.attached()) == "DET_SUF,ATT_PRE");
    BOOST_CHECK(e2.isPrefix() == true);
    BOOST_CHECK(e2.isSuffix() == true);
    BOOST_CHECK(e2.isAttached() == true);
    BOOST_CHECK(e2.isDetached() == true);
    BOOST_CHECK(e2.isPrefixAttached() == true);
    BOOST_CHECK(e2.isSuffixAttached() == false);

    // operator==
    //os.str("");
    //os << e;
    //printf("%s\n", os.str().c_str());
    //os.str("");
    //os << e2;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(not (e == e2));
    BOOST_CHECK( e != e2 );

    LexEntry e3("LEXENTRY:\tFOO\tFOOBAR\tWORD,CITY\tDET_PRE,DET_SUF");
    BOOST_CHECK(e == e3);
}

BOOST_FIXTURE_TEST_CASE(LexEntry_setters_test1, TestFixture)
{
    std::ostringstream os;

    LexEntry e;
    os << e;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXENTRY:\t\t\t\t");

    std::string word = "FOOBAR";
    std::string stdword = "FOOBAR";

    e.set(word, stdword, InClass::WORD);
    os.str(""); // clear os
    os << e;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXENTRY:\tFOOBAR\tFOOBAR\tWORD\t");

    word = "FOO";
    e.word(word);
    os.str(""); // clear os
    os << e;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXENTRY:\tFOO\tFOOBAR\tWORD\t");

    stdword = "FURBAT";
    e.stdword(stdword);
    os.str(""); // clear os
    os << e;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXENTRY:\tFOO\tFURBAT\tWORD\t");

    std::set<InClass::Type> type;
    e.type(type);
    os.str(""); // clear os
    os << e;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXENTRY:\tFOO\tFURBAT\t\t");

    e.type(InClass::NUMBER);
    os.str(""); // clear os
    os << e;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXENTRY:\tFOO\tFURBAT\tNUMBER\t");

    e.type(InClass::WORD);
    os.str(""); // clear os
    os << e;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXENTRY:\tFOO\tFURBAT\tNUMBER,WORD\t");

    std::set<InClass::AttachType> attached;
    attached.insert(InClass::DET_PRE);
    attached.insert(InClass::ATT_SUF);
    e.attached(attached);
    os.str(""); // clear os
    os << e;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXENTRY:\tFOO\tFURBAT\tNUMBER,WORD\tDET_PRE,ATT_SUF");

}

// This must match the BOOST_AUTO_TEST_SUITE(ExampleTestSuite) statement
// above and is used to bracket our test cases.

BOOST_AUTO_TEST_SUITE_END()

