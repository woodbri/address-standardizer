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

#define BOOST_TEST_MODULE TokenTestModule

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <string>
#include "token.h"

// The two relevant Boost namespaces for the unit test framework are:
using namespace boost;
using namespace boost::unit_test;

// Provide a name for our suite of tests. This statement is used to bracket
// our test cases.
BOOST_AUTO_TEST_SUITE(TokenTestSuite)

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
BOOST_FIXTURE_TEST_CASE(Token_Constructors, TestFixture)
{
    std::ostringstream os;

    // Verify default constructor with operator<<
    Token t1;
    os << t1;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "TOKEN:\t\t\t\tBADTOKEN\t");

    os.str(""); // clear os
    Token t2("TOKEN:\tFOOBAR\tFOOBAR\tWORD\tBADTOKEN\tDETACH");
    os << t2;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "TOKEN:\tFOOBAR\tFOOBAR\tWORD\tBADTOKEN\t");

    os.str(""); // clear os
    Token t3("FOOBAR\tFOOBAR\tWORD\tBADTOKEN\tDETACH");
    os << t3;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "TOKEN:\tFOOBAR\tFOOBAR\tWORD\tBADTOKEN\t");
}

BOOST_FIXTURE_TEST_CASE(Token_getters, TestFixture)
{
    std::ostringstream os;

    Token t("TOKEN:\tFOO\tFOOBAR\tWORD,TYPE\tBADTOKEN\tDET_PRE,DET_SUF");
    os << t;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "TOKEN:\tFOO\tFOOBAR\tWORD,TYPE\tBADTOKEN\tDET_PRE,DET_SUF");
    BOOST_CHECK(t.text() == "FOO");
    BOOST_CHECK(t.stdtext() == "FOOBAR");
    BOOST_CHECK(t.inclassAsString() == "WORD,TYPE");
    BOOST_CHECK(InClass::asString(t.inclass()) == "WORD,TYPE");
    BOOST_CHECK(t.attachedAsString() == "DET_PRE,DET_SUF");
    BOOST_CHECK(InClass::asString(t.attached()) == "DET_PRE,DET_SUF");
    BOOST_CHECK(t.outclassAsString() == "BADTOKEN");
    BOOST_CHECK(t.outclass() == OutClass::BADTOKEN);

    std::set<InClass::Type> s1;
    s1.insert(InClass::DASH);
    s1.insert(InClass::BADTOKEN);
    s1.insert(InClass::EMDASH);
    BOOST_CHECK(t.isInClass(s1) == false);

    std::set<InClass::Type> s2;
    s2.insert(InClass::DASH);
    s2.insert(InClass::BADTOKEN);
    s2.insert(InClass::EMDASH);
    s2.insert(InClass::WORD);
    BOOST_CHECK(t.isInClass(s2) == true);

    std::set<InClass::Type> s3;

    BOOST_CHECK(t.isInClass(s3) == false);
    s3.insert(InClass::TYPE);
    BOOST_CHECK(t.isInClass(s3) == true);
    s3.insert(InClass::WORD);
    BOOST_CHECK(t.isInClass(s3) == true);
    s3.insert(InClass::EMDASH);
    s3.insert(InClass::EMDASH);
    BOOST_CHECK(t.isInClass(s3) == true);
    s3.insert(InClass::BADTOKEN);

    BOOST_CHECK(t.isInClassEmpty() == false);
}

BOOST_FIXTURE_TEST_CASE(Token_setters_test1, TestFixture)
{
    std::ostringstream os;

    // Verify default constructor with operator<<
    Token t;
    os << t;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "TOKEN:\t\t\t\tBADTOKEN\t");

    // now set the attibutes
    t.text("FOO");
    t.stdtext("FOOBAR");
    t.inclass(InClass::TYPE);
    t.inclass(InClass::WORD);
    t.attached(InClass::DET_SUF);
    t.attached(InClass::DET_PRE);
    
    os.str(""); // clear it
    os << t;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "TOKEN:\tFOO\tFOOBAR\tWORD,TYPE\tBADTOKEN\tDET_PRE,DET_SUF");
}

BOOST_FIXTURE_TEST_CASE(Token_setters_test2, TestFixture)
{
    std::ostringstream os;

    // Verify default constructor with operator<<
    Token t;
    os << t;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "TOKEN:\t\t\t\tBADTOKEN\t");

    // now set the attibutes
    t.text("FOO");
    t.stdtext("FOOBAR");
    t.inclass(InClass::asType("TYPE,WORD"));
    t.attached(InClass::asAttachType("DET_SUF,DET_PRE"));
    
    os.str(""); // clear it
    os << t;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "TOKEN:\tFOO\tFOOBAR\tWORD,TYPE\tBADTOKEN\tDET_PRE,DET_SUF");

    // test outclass(val) setter
    t.outclass(OutClass::EXTRA);

    os.str(""); // clear it
    os << t;
    //printf("%s\n", os.str().c_str());
    BOOST_CHECK(os.str() == "TOKEN:\tFOO\tFOOBAR\tWORD,TYPE\tEXTRA\tDET_PRE,DET_SUF");

}

BOOST_FIXTURE_TEST_CASE(Token_enumerate, TestFixture)
{
    std::ostringstream os;

    // create tokens
    Token t1("TOKEN:\t11\t\tNUMBER\tBADTOKEN\t");
    Token t2("TOKEN:\tRADCLIFF\t\tWORD\tBADTOKEN\t");
    Token t3("TOKEN:\tRD\t\tTYPE\tBADTOKEN\t");
    Token t4("TOKEN:\tNORTH\t\tDIRECT,WORD\tBADTOKEN\t");
    Token t5("TOKEN:\tCHELMSFORD\t\tWORD\tBADTOKEN\t");
    Token t6("TOKEN:\tMA\t\tPROV\tBADTOKEN\t");
    Token t7("TOKEN:\t01863\t\tNUMBER,QUINT\tBADTOKEN\t");
    Token t8("TOKEN:\t2313\t\tNUMBER,QUAD\tBADTOKEN\t");
    Token t9("TOKEN:\tUSA\t\tNATION\tBADTOKEN\t");

    // push them onto a vector
    std::vector<Token> tokens;
    tokens.push_back(t1);
    tokens.push_back(t2);
    tokens.push_back(t3);
    tokens.push_back(t4);
    tokens.push_back(t5);
    tokens.push_back(t6);
    tokens.push_back(t7);
    tokens.push_back(t8);
    tokens.push_back(t9);

    // enumerate the toke inclasses
    std::vector< std::vector<InClass::Type> > result =
        Token::enumerate( tokens );

    // convert the results to a string
    std::string sresult;
    for (const auto &v : result) {
        for (const auto &e : v)
            sresult += InClass::asString(e) + "  ";
        sresult += "\n";
    }

    // define the expected results
    std::string expected(
        "NUMBER  WORD  TYPE  WORD  WORD  PROV  NUMBER  NUMBER  NATION  \n"
        "NUMBER  WORD  TYPE  WORD  WORD  PROV  NUMBER  QUAD  NATION  \n"
        "NUMBER  WORD  TYPE  WORD  WORD  PROV  QUINT  NUMBER  NATION  \n"
        "NUMBER  WORD  TYPE  WORD  WORD  PROV  QUINT  QUAD  NATION  \n"
        "NUMBER  WORD  TYPE  DIRECT  WORD  PROV  NUMBER  NUMBER  NATION  \n"
        "NUMBER  WORD  TYPE  DIRECT  WORD  PROV  NUMBER  QUAD  NATION  \n"
        "NUMBER  WORD  TYPE  DIRECT  WORD  PROV  QUINT  NUMBER  NATION  \n"
        "NUMBER  WORD  TYPE  DIRECT  WORD  PROV  QUINT  QUAD  NATION  \n"
    );

    //printf("expected:\n%s\n", expected.c_str());
    //printf("result:\n%s\n", sresult.c_str());

    BOOST_CHECK(sresult == expected);
}

// This must match the BOOST_AUTO_TEST_SUITE(ExampleTestSuite) statement
// above and is used to bracket our test cases.

BOOST_AUTO_TEST_SUITE_END()

