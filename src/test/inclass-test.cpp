
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

#define BOOST_TEST_MODULE InClassTestModule

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <set>
#include <string>
#include "inclass.h"

// The two relevant Boost namespaces for the unit test framework are:
using namespace boost;
using namespace boost::unit_test;

// Provide a name for our suite of tests. This statement is used to bracket
// our test cases.
BOOST_AUTO_TEST_SUITE(InClassTestSuite)

// The structure below allows us to pass a test initialization object to
// each test case. Note the use of struct to default all methods and member
// variables to public access.
struct TestFixture
{
    TestFixture() : i(0) {
        // Put test initialization here, the constructor will be called
        // prior to the execution of each test case
        //printf("Initialize test\n");
        types.insert(InClass::EMDASH);
        types.insert(InClass::MIXED);
        types.insert(InClass::TYPE);
        types.insert(InClass::WORD);

        attached.insert(InClass::ATT_SUF);
        attached.insert(InClass::DET_PRE);
        attached.insert(InClass::DET_SUF);
    }
    ~TestFixture() {
        // Put test cleanup here, the destructor will automatically be
        // invoked at the end of each test case.
        //printf("Cleanup test\n");
    }
    // Public test fixture variables are automatically available to all test
    // cases. Don’t forget to initialize these variables in the constructors
    // to avoid initialized variable errors.
    std::set<InClass::Type> types;
    std::set<InClass::AttachType> attached;

    int i;
};

// Define a test case. The first argument specifies the name of the test.
// Take some care in naming your tests. Do not reuse names or accidentally use
// the same name for a test as specified for the module test suite name.
//
// The second argument provides a test build-up/tear-down object that is
// responsible for creating and destroying any resources needed by the
// unit test
BOOST_FIXTURE_TEST_CASE(InClass_asString, TestFixture)
{
    // Verify asString for single Type
    BOOST_REQUIRE(InClass::asString(InClass::STOP) == "STOP");
    BOOST_REQUIRE(InClass::asString(InClass::NUMBER) == "NUMBER");
    BOOST_REQUIRE(InClass::asString(InClass::WORD) == "WORD");
    BOOST_REQUIRE(InClass::asString(InClass::TYPE) == "TYPE");
    BOOST_REQUIRE(InClass::asString(InClass::QUALIF) == "QUALIF");
    BOOST_REQUIRE(InClass::asString(InClass::ROAD) == "ROAD");
    BOOST_REQUIRE(InClass::asString(InClass::STOPWORD) == "STOPWORD");
    BOOST_REQUIRE(InClass::asString(InClass::RR) == "RR");
    BOOST_REQUIRE(InClass::asString(InClass::DASH) == "DASH");
    BOOST_REQUIRE(InClass::asString(InClass::CITY) == "CITY");
    BOOST_REQUIRE(InClass::asString(InClass::PROV) == "PROV");
    BOOST_REQUIRE(InClass::asString(InClass::NATION) == "NATION");
    BOOST_REQUIRE(InClass::asString(InClass::AMPERS) == "AMPERS");
    BOOST_REQUIRE(InClass::asString(InClass::BOXH) == "BOXH");
    BOOST_REQUIRE(InClass::asString(InClass::ORD) == "ORD");
    BOOST_REQUIRE(InClass::asString(InClass::UNITH) == "UNITH");
    BOOST_REQUIRE(InClass::asString(InClass::SINGLE) == "SINGLE");
    BOOST_REQUIRE(InClass::asString(InClass::BUILDH) == "BUILDH");
    BOOST_REQUIRE(InClass::asString(InClass::MILE) == "MILE");
    BOOST_REQUIRE(InClass::asString(InClass::DOUBLE) == "DOUBLE");
    BOOST_REQUIRE(InClass::asString(InClass::DIRECT) == "DIRECT");
    BOOST_REQUIRE(InClass::asString(InClass::MIXED) == "MIXED");
    BOOST_REQUIRE(InClass::asString(InClass::BUILDT) == "BUILDT");
    BOOST_REQUIRE(InClass::asString(InClass::FRACT) == "FRACT");
    BOOST_REQUIRE(InClass::asString(InClass::PCT) == "PCT");
    BOOST_REQUIRE(InClass::asString(InClass::PCH) == "PCH");
    BOOST_REQUIRE(InClass::asString(InClass::QUINT) == "QUINT");
    BOOST_REQUIRE(InClass::asString(InClass::QUAD) == "QUAD");
    BOOST_REQUIRE(InClass::asString(InClass::PUNCT) == "PUNCT");
    BOOST_REQUIRE(InClass::asString(InClass::SPACE) == "SPACE");
    BOOST_REQUIRE(InClass::asString(InClass::PLACEN) == "PLACEN");
    BOOST_REQUIRE(InClass::asString(InClass::EMDASH) == "EMDASH");
    BOOST_REQUIRE(InClass::asString(InClass::BADTOKEN) == "BADTOKEN");

    // Verify the a set converts to comma separated list (sorted)
    // Verify types is "EMDASH,MIXED,TYPE,WORD"
    std::string str_types = InClass::asString(types);
    //printf("%s\n", str_types.c_str());
    BOOST_REQUIRE(str_types == "WORD,TYPE,MIXED,EMDASH");

    // Verify asString for single attachType
    BOOST_REQUIRE(InClass::asString(InClass::DET_PRE) == "DET_PRE");
    BOOST_REQUIRE(InClass::asString(InClass::DET_SUF) == "DET_SUF");
    BOOST_REQUIRE(InClass::asString(InClass::ATT_PRE) == "ATT_PRE");
    BOOST_REQUIRE(InClass::asString(InClass::ATT_SUF) == "ATT_SUF");

    // Verify asString for set of attachType
    std::string str_attached = InClass::asString(attached);
    //printf("%s\n", str_attached.c_str());
    BOOST_REQUIRE(str_attached == "DET_PRE,DET_SUF,ATT_SUF");
}

BOOST_FIXTURE_TEST_CASE(InClass_asType, TestFixture)
{
    // verify conversion of string to InClass::Type
    BOOST_REQUIRE(InClass::asOneType("STOP")      == InClass::STOP);
    BOOST_REQUIRE(InClass::asOneType("NUMBER")    == InClass::NUMBER);
    BOOST_REQUIRE(InClass::asOneType("WORD")      == InClass::WORD);
    BOOST_REQUIRE(InClass::asOneType("TYPE")      == InClass::TYPE);
    BOOST_REQUIRE(InClass::asOneType("QUALIF")    == InClass::QUALIF);
    BOOST_REQUIRE(InClass::asOneType("ROAD")      == InClass::ROAD);
    BOOST_REQUIRE(InClass::asOneType("STOPWORD")  == InClass::STOPWORD);
    BOOST_REQUIRE(InClass::asOneType("RR")        == InClass::RR);
    BOOST_REQUIRE(InClass::asOneType("DASH")      == InClass::DASH);
    BOOST_REQUIRE(InClass::asOneType("CITY")      == InClass::CITY);
    BOOST_REQUIRE(InClass::asOneType("PROV")      == InClass::PROV);
    BOOST_REQUIRE(InClass::asOneType("NATION")    == InClass::NATION);
    BOOST_REQUIRE(InClass::asOneType("AMPERS")    == InClass::AMPERS);
    BOOST_REQUIRE(InClass::asOneType("BOXH")      == InClass::BOXH);
    BOOST_REQUIRE(InClass::asOneType("ORD")       == InClass::ORD);
    BOOST_REQUIRE(InClass::asOneType("UNITH")     == InClass::UNITH);
    BOOST_REQUIRE(InClass::asOneType("SINGLE")    == InClass::SINGLE);
    BOOST_REQUIRE(InClass::asOneType("BUILDH")    == InClass::BUILDH);
    BOOST_REQUIRE(InClass::asOneType("MILE")      == InClass::MILE);
    BOOST_REQUIRE(InClass::asOneType("DOUBLE")    == InClass::DOUBLE);
    BOOST_REQUIRE(InClass::asOneType("DIRECT")    == InClass::DIRECT);
    BOOST_REQUIRE(InClass::asOneType("MIXED")     == InClass::MIXED);
    BOOST_REQUIRE(InClass::asOneType("BUILDT")    == InClass::BUILDT);
    BOOST_REQUIRE(InClass::asOneType("FRACT")     == InClass::FRACT);
    BOOST_REQUIRE(InClass::asOneType("PCT")       == InClass::PCT);
    BOOST_REQUIRE(InClass::asOneType("PCH")       == InClass::PCH);
    BOOST_REQUIRE(InClass::asOneType("QUINT")     == InClass::QUINT);
    BOOST_REQUIRE(InClass::asOneType("QUAD")      == InClass::QUAD);
    BOOST_REQUIRE(InClass::asOneType("PUNCT")     == InClass::PUNCT);
    BOOST_REQUIRE(InClass::asOneType("SPACE")     == InClass::SPACE);
    BOOST_REQUIRE(InClass::asOneType("PLACEN")    == InClass::PLACEN);
    BOOST_REQUIRE(InClass::asOneType("EMDASH")    == InClass::EMDASH);
    BOOST_REQUIRE(InClass::asOneType("BADTOKEN")  == InClass::BADTOKEN);
    BOOST_REQUIRE(InClass::asOneType("")          == InClass::BADTOKEN);
    BOOST_REQUIRE(InClass::asOneType("DET_PRE")   == InClass::BADTOKEN);

    // verify conversion of string to InClass::AttachType
    std::set<InClass::AttachType> tmp;

    // Verify that words not in the enum do nothing
    tmp = InClass::asAttachType("DETACH");
    BOOST_REQUIRE(InClass::asString(tmp) == "");

    // check individual AttachType conversions
    tmp = InClass::asAttachType("DET_PRE");
    BOOST_REQUIRE(InClass::asString(tmp) == "DET_PRE");

    tmp = InClass::asAttachType("DET_SUF");
    BOOST_REQUIRE(InClass::asString(tmp) == "DET_SUF");

    tmp = InClass::asAttachType("ATT_PRE");
    BOOST_REQUIRE(InClass::asString(tmp) == "ATT_PRE");

    tmp = InClass::asAttachType("ATT_SUF");
    BOOST_REQUIRE(InClass::asString(tmp) == "ATT_SUF");

    // verify conversion of a list with good and bad tokens
    tmp = InClass::asAttachType("DETACH,DET_PRE,DET_SUF,ATT_PRE,ATT_SUF");
    std::string tmp2 = InClass::asString(tmp);
    //printf("%s\n", tmp2.c_str());
    BOOST_REQUIRE(tmp2 == "DET_PRE,DET_SUF,ATT_PRE,ATT_SUF");

    // verify conversion of a list with good and bad tokens and spaces
    tmp = InClass::asAttachType(" DETACH ,DET_PRE, DET_SUF , ATT_PRE,ATT_SUF ");
    std::string tmp3 = InClass::asString(tmp);
    //printf("%s\n", tmp3.c_str());
    BOOST_REQUIRE(tmp3 == "DET_PRE,DET_SUF,ATT_PRE,ATT_SUF");

}

BOOST_FIXTURE_TEST_CASE(InClass_asNum, TestFixture)
{
    // verify conversion of string to InClass::Type
    BOOST_REQUIRE(InClass::asType(-1) == InClass::STOP);
    BOOST_REQUIRE(InClass::asType(0)  == InClass::NUMBER);
    BOOST_REQUIRE(InClass::asType(1)  == InClass::WORD);
    BOOST_REQUIRE(InClass::asType(2)  == InClass::TYPE);
    BOOST_REQUIRE(InClass::asType(3)  == InClass::QUALIF);
    BOOST_REQUIRE(InClass::asType(4)  == InClass::BADTOKEN);
    BOOST_REQUIRE(InClass::asType(5)  == InClass::BADTOKEN);
    BOOST_REQUIRE(InClass::asType(6)  == InClass::ROAD);
    BOOST_REQUIRE(InClass::asType(7)  == InClass::STOPWORD);
    BOOST_REQUIRE(InClass::asType(8)  == InClass::RR);
    BOOST_REQUIRE(InClass::asType(9)  == InClass::DASH);
    BOOST_REQUIRE(InClass::asType(10) == InClass::CITY);
    BOOST_REQUIRE(InClass::asType(11) == InClass::PROV);
    BOOST_REQUIRE(InClass::asType(12) == InClass::NATION);
    BOOST_REQUIRE(InClass::asType(13) == InClass::AMPERS);
    BOOST_REQUIRE(InClass::asType(14) == InClass::BOXH);
    BOOST_REQUIRE(InClass::asType(15) == InClass::ORD);
    BOOST_REQUIRE(InClass::asType(16) == InClass::UNITH);
    BOOST_REQUIRE(InClass::asType(17) == InClass::BADTOKEN);
    BOOST_REQUIRE(InClass::asType(18) == InClass::SINGLE);
    BOOST_REQUIRE(InClass::asType(19) == InClass::BUILDH);
    BOOST_REQUIRE(InClass::asType(20) == InClass::MILE);
    BOOST_REQUIRE(InClass::asType(21) == InClass::DOUBLE);
    BOOST_REQUIRE(InClass::asType(22) == InClass::DIRECT);
    BOOST_REQUIRE(InClass::asType(23) == InClass::MIXED);
    BOOST_REQUIRE(InClass::asType(24) == InClass::BUILDT);
    BOOST_REQUIRE(InClass::asType(25) == InClass::FRACT);
    BOOST_REQUIRE(InClass::asType(26) == InClass::PCT);
    BOOST_REQUIRE(InClass::asType(27) == InClass::PCH);
    BOOST_REQUIRE(InClass::asType(28) == InClass::QUINT);
    BOOST_REQUIRE(InClass::asType(29) == InClass::QUAD);
    BOOST_REQUIRE(InClass::asType(30) == InClass::PUNCT);
    BOOST_REQUIRE(InClass::asType(31) == InClass::SPACE);
    BOOST_REQUIRE(InClass::asType(32) == InClass::PLACEN);
    BOOST_REQUIRE(InClass::asType(33) == InClass::EMDASH);
    BOOST_REQUIRE(InClass::asType(99) == InClass::BADTOKEN);
    
}

BOOST_FIXTURE_TEST_CASE(InClass_Lang, TestFixture)
{
    // TODO
}

// This must match the BOOST_AUTO_TEST_SUITE(ExampleTestSuite) statement
// above and is used to bracket our test cases.

BOOST_AUTO_TEST_SUITE_END()

