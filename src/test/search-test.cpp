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

#define BOOST_TEST_MODULE SearchTestModule

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <string>
#include "token.h"
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
            "[ADDRESS]\n"
            "@AB @CD @EF\n"
            "@A @BC @DE\n"
            "@CD @EF\n\n"
            "[A]\n"
            "NUMBER -> BLDNG -> 0.5\n\n"
            "[AB]\n"
            "NUMBER WORD -> BLDNG HOUSE -> 0.5\n\n"
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
    // define sequence of tokens we want to search for in the grammar
    std::vector<Token> pat1;
    pat1.push_back( Token("11\t11\tNUMBER\tBADTOKEN\tDETACH") );
    pat1.push_back( Token("OAK\tOAK\tWORD\tBADTOKEN\tDETACH") );
    pat1.push_back( Token("ST\tSTREET\tTYPE\tBADTOKEN\tDETACH") );
    pat1.push_back( Token("EXT\tEXT\tQUALIF\tBADTOKEN\tDETACH") );
    pat1.push_back( Token("HWY\tHWY\tROAD\tBADTOKEN\tDETACH") );

    Search s(G);
    SearchPaths mr = s.search( pat1 );
    //printf("mr.size: %ld\n", mr.size() );
    BOOST_CHECK(mr.size() == 1);

    std::string expect =
        "NUMBER -> BLDNG -> 0.5\n"
        "WORD TYPE -> HOUSE PREDIR -> 0.5\n"
        "QUALIF ROAD -> QUALIF SUFTYP -> 0.5\n";

    os.str(""); // clear
    for (const auto &e : mr) {
        os << resultAsString( e.rules );
    }
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == expect);

}

BOOST_FIXTURE_TEST_CASE(SearchTest_2, TestFixture)
{
    // define sequence of tokens we want to search for in the grammar
    std::vector<Token> pat1;
    pat1.push_back( Token("11\t11\tNUMBER\tBADTOKEN\tDETACH") );
    pat1.push_back( Token("OAK\tOAK\tWORD\tBADTOKEN\tDETACH") );
    pat1.push_back( Token("ST\tSTREET\tTYPE\tBADTOKEN\tDETACH") );
    pat1.push_back( Token("EXT\tEXT\tQUALIF\tBADTOKEN\tDETACH") );
    pat1.push_back( Token("HWY\tHWY\tROAD\tBADTOKEN\tDETACH") );

    Search s(G);
    float cost = 0.0;
    float bestNrules = -1.0;
    std::string matched;
    auto mr = s.searchAndReclassBest( pat1, cost, matched, bestNrules );
    //printf("s.searchAndReclassBest: cost: %.3f\n", cost);
    BOOST_CHECK_CLOSE( cost, 0.500, 0.001 );

/*
    std::string expect =
        "NUMBER -> BLDNG -> 0.5\n"
        "WORD TYPE -> HOUSE PREDIR -> 0.5\n"
        "QUALIF ROAD -> QUALIF SUFTYP -> 0.5\n";

    os.str(""); // clear
    os << resultAsString( mr.rules );

    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == expect);
*/

    std::string expect2 =
        "TOKEN:\t11\t11\tNUMBER\tBLDNG\t\n"
        "TOKEN:\tOAK\tOAK\tWORD\tHOUSE\t\n"
        "TOKEN:\tST\tSTREET\tTYPE\tPREDIR\t\n"
        "TOKEN:\tEXT\tEXT\tQUALIF\tQUALIF\t\n"
        "TOKEN:\tHWY\tHWY\tROAD\tSUFTYP\t\n";

    os.str(""); // clear
    for ( const auto &e : mr )
        os << e << "\n";
    //printf("'%s'\n'%s'\n", os.str().c_str(), expect2.c_str());
    BOOST_CHECK(os.str() == expect2);
}

BOOST_FIXTURE_TEST_CASE(SearchTest_3, TestFixture)
{
    std::vector<Token> pat3;
    pat3.push_back( Token("11\t11\tNUMBER\tBADTOKEN\tDETACH") );
    pat3.push_back( Token("OAK\tOAK\tWORD\tBADTOKEN\tDETACH") );
    pat3.push_back( Token("ST\tSTREET\tTYPE\tBADTOKEN\tDETACH") );
    pat3.push_back( Token("EXT\tEXT\tQUALIF\tBADTOKEN\tDETACH") );
    pat3.push_back( Token("HWY\tHWY\tROAD\tBADTOKEN\tDETACH") );
    pat3.push_back( Token("RR\tRR\tRR\tBADTOKEN\tDETACH") );


    Search s(G);
    SearchPaths mr = s.search( pat3 );
    BOOST_CHECK(mr.size() == 1);

    std::string expect =
        "NUMBER WORD -> BLDNG HOUSE -> 0.5\n"
        "TYPE QUALIF -> PREDIR QUALIF -> 0.5\n"
        "ROAD RR -> SUFTYP SUFDIR -> 0.5\n";

    os.str(""); // clear
    for (const auto &e : mr) {
        os << resultAsString( e.rules );
    }
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == expect);

}

BOOST_FIXTURE_TEST_CASE(SearchTest_4, TestFixture)
{
    std::vector<Token> pat4;
    pat4.push_back( Token("11\t11\tNUMBER\tBADTOKEN\tDETACH") );
    pat4.push_back( Token("OAK\tOAK\tWORD\tBADTOKEN\tDETACH") );
    pat4.push_back( Token("ST\tSTREET\tTYPE\tBADTOKEN\tDETACH") );
    pat4.push_back( Token("HWY\tHWY\tROAD\tBADTOKEN\tDETACH") );


    Search s(G);
    SearchPaths mr = s.search( pat4 );
    BOOST_CHECK(mr.size() == 0);

    std::string expect = "";

    os.str(""); // clear
    for (const auto &e : mr) {
        os << resultAsString( e.rules );
    }
    //printf("'%s'\n", os.str().c_str());
    BOOST_CHECK(os.str() == expect);

}

// This must match the BOOST_AUTO_TEST_SUITE(ExampleTestSuite) statement
// above and is used to bracket our test cases.

BOOST_AUTO_TEST_SUITE_END()

