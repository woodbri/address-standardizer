/**ADDRESS_STANDARDIZER***************************************************
 *
 * Address Standardizer
 *      A collection of C++ classes for parsing street addresses
 *      and standardizing them for the purpose of Geocoding.
 *
 * Copyright 2016 Stephen Woodbridge <woodbri@imaptools.com>
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the MIT License. Please file LICENSE for details.
 *
 ***************************************************ADDRESS_STANDARDIZER**/

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

#define BOOST_TEST_MODULE LexiconTestModule

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <string>
#include "lexentry.h"
#include "token.h"
#include "lexicon.h"

// The two relevant Boost namespaces for the unit test framework are:
using namespace boost;
using namespace boost::unit_test;

// Provide a name for our suite of tests. This statement is used to bracket
// our test cases.
BOOST_AUTO_TEST_SUITE(LexiconTestSuite)

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
            "LEXENTRY:\tALLY\tALY\tTYPE\tDET_SUF\n")
    {
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
    
    std::string lexi;


};

// Define a test case. The first argument specifies the name of the test.
// Take some care in naming your tests. Do not reuse names or accidentally use
// the same name for a test as specified for the module test suite name.
//
// The second argument provides a test build-up/tear-down object that is
// responsible for creating and destroying any resources needed by the
// unit test
BOOST_FIXTURE_TEST_CASE(Lexicon_Constructors, TestFixture)
{
    std::ostringstream os;

    // Verify default constructor with operator<<
    Lexicon lex1;
    os << lex1;
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXICON:\t\tUNKNOWN\t\t0\n");

    Lexicon lex2("test-lexicon");
    os.str(""); // clear os
    os << lex2;
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXICON:\ttest-lexicon\tUNKNOWN\t\t0\n");

    Lexicon lex3("text-lexicon-2", "lex-test.txt");
    os.str(""); // clear os
    os << lex3;
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == lexi);
}

BOOST_FIXTURE_TEST_CASE(Lexicon_getters, TestFixture)
{
    std::ostringstream os;

    Lexicon lex("text-lexicon-2", "lex-test.txt");
    os << lex;
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == lexi);

    BOOST_CHECK(lex.name() == "text-lexicon-2");
    BOOST_CHECK(lex.lang() == InClass::ENG);
    BOOST_CHECK(lex.langAsString() == "ENG");
    BOOST_CHECK(lex.langAsName() == "ENGLISH");
    BOOST_CHECK(lex.locale() == "en_US");

    // check for and entry in the lexicon
    std::vector<LexEntry> le = lex.find("ALLEE");
    BOOST_CHECK(le.size() == 1);
    os.str(""); // clear os
    os << le[0];
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXENTRY:\tALLEE\tALY\tTYPE\tDET_SUF");

    // check the regex
    std::string rg1 = lex.regex();
    //printf("%s\n", rg1.c_str());
    BOOST_CHECK(rg1 == "ALLEE|ALLEY|ALLY|AK|AL|");

    // check regexPrefixAtt
    std::string rg2 = lex.regexPrefixAtt();
    //printf("%s\n", rg2.c_str());
    BOOST_CHECK(rg2 == "");

    // check regexSuffixAtt
    std::string rg3 = lex.regexSuffixAtt();
    //printf("%s\n", rg3.c_str());
    BOOST_CHECK(rg3 == "");

}

BOOST_FIXTURE_TEST_CASE(Lexicon_mutators_test1, TestFixture)
{
    std::ostringstream os;

    Lexicon lex;
    os << lex;
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXICON:\t\tUNKNOWN\t\t0\n");

    lex.name("test-lexicon-2");
    lex.lang(InClass::GER);
    lex.locale("de_DE");
    os.str(""); // clear os
    os << lex;
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXICON:\ttest-lexicon-2\tGER\tde_DE\t0\n");

    lex.lang("ENG");
    lex.locale("en_US");
    os.str(""); // clear os
    os << lex;
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXICON:\ttest-lexicon-2\tENG\ten_US\t0\n");

    LexEntry le1("LEXENTRY:\tFOOBAR\tFOOBAR\tNUMBER\tDET_PRE,DET_SUF");
    os.str(""); // clear os
    os << le1;
    BOOST_CHECK(os.str() == "LEXENTRY:\tFOOBAR\tFOOBAR\tNUMBER\tDET_PRE,DET_SUF");

    // add the entry and check it
    lex.insert(le1);
    os.str(""); // clear os
    os << lex;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXICON:\ttest-lexicon-2\tENG\ten_US\t1\nLEXENTRY:\tFOOBAR\tFOOBAR\tNUMBER\tDET_PRE,DET_SUF\n");

    LexEntry le2("LEXENTRY:\tCHELMSFORD\tCHELMSFORD\tWORD,CITY\tDETACH");
    os.str(""); // clear os
    os << le2;
    BOOST_CHECK(os.str() == "LEXENTRY:\tCHELMSFORD\tCHELMSFORD\tWORD,CITY\t");

    // add the entry and check it
    lex.insert(le2);
    os.str(""); // clear os
    os << lex;
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXICON:\ttest-lexicon-2\tENG\ten_US\t2\nLEXENTRY:\tCHELMSFORD\tCHELMSFORD\tWORD,CITY\t\nLEXENTRY:\tFOOBAR\tFOOBAR\tNUMBER\tDET_PRE,DET_SUF\n");

    Token tok("FOOBAR\tFOOBAR\t\tBADTOKEN\t");
    os.str(""); // clear os
    os << tok;
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == "TOKEN:\tFOOBAR\tFOOBAR\t\tBADTOKEN\t");

    lex.classify( tok, InClass::WORD );
    os.str(""); // clear os
    os << tok;
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == "TOKEN:\tFOOBAR\tFOOBAR\tNUMBER\tBADTOKEN\t");

    Token tok2("TOKEN:\tFOOFOO\tFOOFOO\t\tBADTOKEN\t");
    os.str(""); // clear os
    os << tok2;
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == "TOKEN:\tFOOFOO\tFOOFOO\t\tBADTOKEN\t");

    lex.classify( tok2, InClass::WORD );
    os.str(""); // clear os
    os << tok2;
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == "TOKEN:\tFOOFOO\tFOOFOO\tWORD\tBADTOKEN\t");

    lex.remove( le1 );
    os.str(""); // clear os
    os << lex;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "LEXICON:\ttest-lexicon-2\tENG\ten_US\t1\nLEXENTRY:\tCHELMSFORD\tCHELMSFORD\tWORD,CITY\t\n");

}

// This must match the BOOST_AUTO_TEST_SUITE(ExampleTestSuite) statement
// above and is used to bracket our test cases.

BOOST_AUTO_TEST_SUITE_END()

