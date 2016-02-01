
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
#include "token.h"
#include "tokenizer.h"

// The two relevant Boost namespaces for the unit test framework are:
using namespace boost;
using namespace boost::unit_test;

// Provide a name for our suite of tests. This statement is used to bracket
// our test cases.
BOOST_AUTO_TEST_SUITE(TokenizerTestSuite)

// The structure below allows us to pass a test initialization object to
// each test case. Note the use of struct to default all methods and member
// variables to public access.
struct TestFixture
{
    TestFixture() :
        lexi(
            "LEXICON:\ttext-lexicon-2\tENG\ten_US\t5\n"
            "LEXENTRY:\tAK\tALASKA\tPROV\t\n"
            "LEXENTRY:\tAL\tALABAMA\tPROV\t\n"
            "LEXENTRY:\tALLEE\tALY\tTYPE\tDET_SUF\n"
            "LEXENTRY:\tALLEY\tALY\tTYPE\tDET_SUF\n"
            "LEXENTRY:\tALLY\tALY\tTYPE\tDET_SUF\n"),
        lexicon("text-lexicon-2", "lex-test.txt")
    {
        // Put test initialization here, the constructor will be called
        // prior to the execution of each test case
        //printf("Initialize test\n");

        //verify lexicon loaded correctly
        os.str(""); // clear os
        os << lexicon;
        BOOST_CHECK(os.str() == lexi);
    }
    ~TestFixture() {
        // Put test cleanup here, the destructor will automatically be
        // invoked at the end of each test case.
        //printf("Cleanup test\n");
    }
    // Public test fixture variables are automatically available to all test
    // cases. Don’t forget to initialize these variables in the constructors
    // to avoid initialized variable errors.
    std::string lexi;
    Lexicon lexicon;
    std::ostringstream os;
};

// Define a test case. The first argument specifies the name of the test.
// Take some care in naming your tests. Do not reuse names or accidentally use
// the same name for a test as specified for the module test suite name.
//
// The second argument provides a test build-up/tear-down object that is
// responsible for creating and destroying any resources needed by the
// unit test
BOOST_FIXTURE_TEST_CASE(Tokenizer_Testing1, TestFixture)
{
    // create a tokenizer using the test lexicon
    Tokenizer tz(lexicon);

    // parse a string
    std::vector<Token> tokens = tz.getTokens("11 Dackon Allee Podunk AL 12345");

    os.str(""); // clear
    for (const auto &t : tokens)
        os << t << "\n";
    //printf("'%s'\n", os.str().c_str());
    std::string expect1 =
        "TOKEN:\t11\t\tNUMBER\tBADTOKEN\t\n"
        "TOKEN:\t \t\tPUNCT\tBADTOKEN\t\n"
        "TOKEN:\tDACKON\t\tWORD\tBADTOKEN\t\n"
        "TOKEN:\t \t\tPUNCT\tBADTOKEN\t\n"
        "TOKEN:\tALLEE\t\tTYPE\tBADTOKEN\t\n"
        "TOKEN:\t \t\tPUNCT\tBADTOKEN\t\n"
        "TOKEN:\tPODUNK\t\tWORD\tBADTOKEN\t\n"
        "TOKEN:\t \t\tPUNCT\tBADTOKEN\t\n"
        "TOKEN:\tAL\t\tPROV\tBADTOKEN\t\n"
        "TOKEN:\t \t\tPUNCT\tBADTOKEN\t\n"
        "TOKEN:\t12345\t\tNUMBER,QUINT\tBADTOKEN\t\n";
    BOOST_CHECK(os.str() == expect1);

    // get the tokens with default classifications for a null string
    tokens.clear();
    tokens = tz.getTokens("");

    os.str(""); // clear
    for (const auto &t : tokens)
        os << t << "\n";
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == "");

    // filter out PUNCT tokens
    tz.addFilter(InClass::PUNCT);
    tokens = tz.getTokens("11 Dackon Allee Podunk AL 12345");

    os.str(""); // clear
    for (const auto &t : tokens)
        os << t << "\n";
    //printf("'%s'\n", os.str().c_str());
    std::string expect2 =
        "TOKEN:\t11\t\tNUMBER\tBADTOKEN\t\n"
        "TOKEN:\tDACKON\t\tWORD\tBADTOKEN\t\n"
        "TOKEN:\tALLEE\t\tTYPE\tBADTOKEN\t\n"
        "TOKEN:\tPODUNK\t\tWORD\tBADTOKEN\t\n"
        "TOKEN:\tAL\t\tPROV\tBADTOKEN\t\n"
        "TOKEN:\t12345\t\tNUMBER,QUINT\tBADTOKEN\t\n";
    BOOST_CHECK(os.str() == expect2);

    // filter out PUNCT tokens  and QUINT
    tz.addFilter(InClass::QUINT);
    tokens = tz.getTokens("11 Dackon Allee Podunk al 12345");

    os.str(""); // clear
    for (const auto &t : tokens)
        os << t << "\n";
    //printf("'%s'\n", os.str().c_str());
    std::string expect3 =
        "TOKEN:\t11\t\tNUMBER\tBADTOKEN\t\n"
        "TOKEN:\tDACKON\t\tWORD\tBADTOKEN\t\n"
        "TOKEN:\tALLEE\t\tTYPE\tBADTOKEN\t\n"
        "TOKEN:\tPODUNK\t\tWORD\tBADTOKEN\t\n"
        "TOKEN:\tAL\t\tPROV\tBADTOKEN\t\n";
    BOOST_CHECK(os.str() == expect3);

    // save the filter so we can restore it later
    std::set<InClass::Type> filter = tz.filter();
    os.str(""); // clear
    for (const auto &e : filter)
        os << InClass::asString(e) << " ";
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == "QUINT PUNCT ");

    // verify that we can remove QUINT filter
    tz.removeFilter(InClass::QUINT);
    tokens = tz.getTokens("11 Dackon Allee Podunk al 12345");

    os.str(""); // clear
    for (const auto &t : tokens)
        os << t << "\n";
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == expect2);

    // verify that removing a non-existent filter doex nothing
    tz.removeFilter(InClass::PROV);
    tokens = tz.getTokens("11 Dackon Allee Podunk al 12345");

    os.str(""); // clear
    for (const auto &t : tokens)
        os << t << "\n";
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == expect2);

    // verify that we can clear the filters
    tz.clearFilter();
    tokens = tz.getTokens("11 Dackon Allee Podunk al 12345");

    os.str(""); // clear
    for (const auto &t : tokens)
        os << t << "\n";
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == expect1);

    // verify that we can install a previously saved filter
    tz.filter(filter);
    tokens = tz.getTokens("11 Dackon Allee Podunk al 12345");

    os.str(""); // clear
    for (const auto &t : tokens)
        os << t << "\n";
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == expect3);

    // fetch the lexicon and verify it
    Lexicon lex1 = tz.lexicon();

    os.str(""); // clear
    os << lex1;
    BOOST_CHECK(os.str() == lexi);

    //printf("lex regex: '%s'\n", lex1.regex().c_str());
    //printf("regexPrefixAtt regex: '%s'\n", lex1.regexPrefixAtt().c_str());
    //printf("lex regexSuffixAtt: '%s'\n", lex1.regexSuffixAtt().c_str());
}

BOOST_FIXTURE_TEST_CASE(Tokenizer_Testing2, TestFixture)
{
    // create and empty lexicon and tokenizer based on that
    Lexicon emptylexicon;
    Tokenizer tz(emptylexicon);

    // get the tokens with default classifications
    std::vector<Token> tokens = tz.getTokens("11 radcliff rd north chelmsford ma 01863");

    os.str(""); // clear
    for (const auto &t : tokens)
        os << t << "\n";
    //printf("'%s'\n", os.str().c_str());
    std::string expect1 =
        "TOKEN:\t11\t\tNUMBER\tBADTOKEN\t\n"
        "TOKEN:\t \t\tPUNCT\tBADTOKEN\t\n"
        "TOKEN:\tRADCLIFF\t\tWORD\tBADTOKEN\t\n"
        "TOKEN:\t \t\tPUNCT\tBADTOKEN\t\n"
        "TOKEN:\tRD\t\tWORD,DOUBLE\tBADTOKEN\t\n"
        "TOKEN:\t \t\tPUNCT\tBADTOKEN\t\n"
        "TOKEN:\tNORTH\t\tWORD\tBADTOKEN\t\n"
        "TOKEN:\t \t\tPUNCT\tBADTOKEN\t\n"
        "TOKEN:\tCHELMSFORD\t\tWORD\tBADTOKEN\t\n"
        "TOKEN:\t \t\tPUNCT\tBADTOKEN\t\n"
        "TOKEN:\tMA\t\tWORD,DOUBLE\tBADTOKEN\t\n"
        "TOKEN:\t \t\tPUNCT\tBADTOKEN\t\n"
        "TOKEN:\t01863\t\tNUMBER,QUINT\tBADTOKEN\t\n";
    BOOST_CHECK(os.str() == expect1);

    // get the tokens with default classifications for a null string
    tokens.clear();
    tokens = tz.getTokens("");

    os.str(""); // clear
    for (const auto &t : tokens)
        os << t << "\n";
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == "");

    // filter out PUNCT tokens
    tz.addFilter(InClass::PUNCT);
    tokens = tz.getTokens("11 radcliff rd north chelmsford ma 01863");

    os.str(""); // clear
    for (const auto &t : tokens)
        os << t << "\n";
    //printf("'%s'\n", os.str().c_str());
    std::string expect2 =
        "TOKEN:\t11\t\tNUMBER\tBADTOKEN\t\n"
        "TOKEN:\tRADCLIFF\t\tWORD\tBADTOKEN\t\n"
        "TOKEN:\tRD\t\tWORD,DOUBLE\tBADTOKEN\t\n"
        "TOKEN:\tNORTH\t\tWORD\tBADTOKEN\t\n"
        "TOKEN:\tCHELMSFORD\t\tWORD\tBADTOKEN\t\n"
        "TOKEN:\tMA\t\tWORD,DOUBLE\tBADTOKEN\t\n"
        "TOKEN:\t01863\t\tNUMBER,QUINT\tBADTOKEN\t\n";
    BOOST_CHECK(os.str() == expect2);

    // filter out PUNCT tokens  and QUINT
    tz.addFilter(InClass::QUINT);
    tokens = tz.getTokens("11 radcliff rd north chelmsford ma 01863");

    os.str(""); // clear
    for (const auto &t : tokens)
        os << t << "\n";
    //printf("'%s'\n", os.str().c_str());
    std::string expect3 =
        "TOKEN:\t11\t\tNUMBER\tBADTOKEN\t\n"
        "TOKEN:\tRADCLIFF\t\tWORD\tBADTOKEN\t\n"
        "TOKEN:\tRD\t\tWORD,DOUBLE\tBADTOKEN\t\n"
        "TOKEN:\tNORTH\t\tWORD\tBADTOKEN\t\n"
        "TOKEN:\tCHELMSFORD\t\tWORD\tBADTOKEN\t\n"
        "TOKEN:\tMA\t\tWORD,DOUBLE\tBADTOKEN\t\n";
    BOOST_CHECK(os.str() == expect3);

    // save the filter so we can restore it later
    std::set<InClass::Type> filter = tz.filter();
    os.str(""); // clear
    for (const auto &e : filter)
        os << InClass::asString(e) << " ";
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == "QUINT PUNCT ");

    // verify that we can remove QUINT filter
    tz.removeFilter(InClass::QUINT);
    tokens = tz.getTokens("11 radcliff rd north chelmsford ma 01863");

    os.str(""); // clear
    for (const auto &t : tokens)
        os << t << "\n";
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == expect2);

    // verify that removing a non-existent filter doex nothing
    tz.removeFilter(InClass::PROV);
    tokens = tz.getTokens("11 radcliff rd north chelmsford ma 01863");

    os.str(""); // clear
    for (const auto &t : tokens)
        os << t << "\n";
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == expect2);

    // verify that we can clear the filters
    tz.clearFilter();
    tokens = tz.getTokens("11 radcliff rd north chelmsford ma 01863");

    os.str(""); // clear
    for (const auto &t : tokens)
        os << t << "\n";
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == expect1);

    // verify that we can install a previously saved filter
    tz.filter(filter);
    tokens = tz.getTokens("11 radcliff rd north chelmsford ma 01863");

    os.str(""); // clear
    for (const auto &t : tokens)
        os << t << "\n";
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == expect3);

}

// This must match the BOOST_AUTO_TEST_SUITE(ExampleTestSuite) statement
// above and is used to bracket our test cases.

BOOST_AUTO_TEST_SUITE_END()

