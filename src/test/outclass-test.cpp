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

#define BOOST_TEST_MODULE OutClassTestModule

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <string>
#include "outclass.h"

// The two relevant Boost namespaces for the unit test framework are:
using namespace boost;
using namespace boost::unit_test;

// Provide a name for our suite of tests. This statement is used to bracket
// our test cases.
BOOST_AUTO_TEST_SUITE(OutClassTestSuite)

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
BOOST_FIXTURE_TEST_CASE(OutClass_asString, TestFixture)
{
    // Verify asString for single Type
    BOOST_REQUIRE(OutClass::asString(OutClass::STOP)        == "STOP");
    BOOST_REQUIRE(OutClass::asString(OutClass::BLDNG)       == "BLDNG");
    BOOST_REQUIRE(OutClass::asString(OutClass::HOUSE)       == "HOUSE");
    BOOST_REQUIRE(OutClass::asString(OutClass::PREDIR)      == "PREDIR");
    BOOST_REQUIRE(OutClass::asString(OutClass::QUALIF)      == "QUALIF");
    BOOST_REQUIRE(OutClass::asString(OutClass::PRETYP)      == "PRETYP");
    BOOST_REQUIRE(OutClass::asString(OutClass::STREET)      == "STREET");
    BOOST_REQUIRE(OutClass::asString(OutClass::SUFTYP)      == "SUFTYP");
    BOOST_REQUIRE(OutClass::asString(OutClass::SUFDIR)      == "SUFDIR");
    BOOST_REQUIRE(OutClass::asString(OutClass::RR)          == "RR");
    BOOST_REQUIRE(OutClass::asString(OutClass::EXTRA)       == "EXTRA");
    BOOST_REQUIRE(OutClass::asString(OutClass::CITY)        == "CITY");
    BOOST_REQUIRE(OutClass::asString(OutClass::PROV)        == "PROV");
    BOOST_REQUIRE(OutClass::asString(OutClass::NATION)      == "NATION");
    BOOST_REQUIRE(OutClass::asString(OutClass::POSTAL)      == "POSTAL");
    BOOST_REQUIRE(OutClass::asString(OutClass::BOXH)        == "BOXH");
    BOOST_REQUIRE(OutClass::asString(OutClass::BOXT)        == "BOXT");
    BOOST_REQUIRE(OutClass::asString(OutClass::UNITH)       == "UNITH");
    BOOST_REQUIRE(OutClass::asString(OutClass::UNITT)       == "UNITT");
    BOOST_REQUIRE(OutClass::asString(OutClass::BADTOKEN)    == "BADTOKEN");

}

BOOST_FIXTURE_TEST_CASE(OutClass_asType, TestFixture)
{
    // verify conversion of string to OutClass::Type
    BOOST_REQUIRE(OutClass::asType("STOP")      == OutClass::STOP);
    BOOST_REQUIRE(OutClass::asType("BLDNG")     == OutClass::BLDNG);
    BOOST_REQUIRE(OutClass::asType("HOUSE")     == OutClass::HOUSE);
    BOOST_REQUIRE(OutClass::asType("PREDIR")    == OutClass::PREDIR);
    BOOST_REQUIRE(OutClass::asType("QUALIF")    == OutClass::QUALIF);
    BOOST_REQUIRE(OutClass::asType("PRETYP")    == OutClass::PRETYP);
    BOOST_REQUIRE(OutClass::asType("STREET")    == OutClass::STREET);
    BOOST_REQUIRE(OutClass::asType("SUFTYP")    == OutClass::SUFTYP);
    BOOST_REQUIRE(OutClass::asType("SUFDIR")    == OutClass::SUFDIR);
    BOOST_REQUIRE(OutClass::asType("RR")        == OutClass::RR);
    BOOST_REQUIRE(OutClass::asType("EXTRA")     == OutClass::EXTRA);
    BOOST_REQUIRE(OutClass::asType("CITY")      == OutClass::CITY);
    BOOST_REQUIRE(OutClass::asType("PROV")      == OutClass::PROV);
    BOOST_REQUIRE(OutClass::asType("NATION")    == OutClass::NATION);
    BOOST_REQUIRE(OutClass::asType("POSTAL")    == OutClass::POSTAL);
    BOOST_REQUIRE(OutClass::asType("BOXH")      == OutClass::BOXH);
    BOOST_REQUIRE(OutClass::asType("BOXT")      == OutClass::BOXT);
    BOOST_REQUIRE(OutClass::asType("UNITH")     == OutClass::UNITH);
    BOOST_REQUIRE(OutClass::asType("UNITT")     == OutClass::UNITT);
    BOOST_REQUIRE(OutClass::asType("BADTOKEN")  == OutClass::BADTOKEN);
    BOOST_REQUIRE(OutClass::asType("FOOBAR")    == OutClass::BADTOKEN);

}

BOOST_FIXTURE_TEST_CASE(OutClass_asNum, TestFixture)
{
    // verify conversion of string to OutClass::Type
    BOOST_REQUIRE(OutClass::asType(-1) == OutClass::STOP);
    BOOST_REQUIRE(OutClass::asType(0)  == OutClass::BLDNG);
    BOOST_REQUIRE(OutClass::asType(1)  == OutClass::HOUSE);
    BOOST_REQUIRE(OutClass::asType(2)  == OutClass::PREDIR);
    BOOST_REQUIRE(OutClass::asType(3)  == OutClass::QUALIF);
    BOOST_REQUIRE(OutClass::asType(4)  == OutClass::PRETYP);
    BOOST_REQUIRE(OutClass::asType(5)  == OutClass::STREET);
    BOOST_REQUIRE(OutClass::asType(6)  == OutClass::SUFTYP);
    BOOST_REQUIRE(OutClass::asType(7)  == OutClass::SUFDIR);
    BOOST_REQUIRE(OutClass::asType(8)  == OutClass::RR);
    BOOST_REQUIRE(OutClass::asType(9)  == OutClass::EXTRA);
    BOOST_REQUIRE(OutClass::asType(10) == OutClass::CITY);
    BOOST_REQUIRE(OutClass::asType(11) == OutClass::PROV);
    BOOST_REQUIRE(OutClass::asType(12) == OutClass::NATION);
    BOOST_REQUIRE(OutClass::asType(13) == OutClass::POSTAL);
    BOOST_REQUIRE(OutClass::asType(14) == OutClass::BOXH);
    BOOST_REQUIRE(OutClass::asType(15) == OutClass::BOXT);
    BOOST_REQUIRE(OutClass::asType(16) == OutClass::UNITH);
    BOOST_REQUIRE(OutClass::asType(17) == OutClass::UNITT);
    BOOST_REQUIRE(OutClass::asType(99) == OutClass::BADTOKEN);
    
}

// This must match the BOOST_AUTO_TEST_SUITE(ExampleTestSuite) statement
// above and is used to bracket our test cases.

BOOST_AUTO_TEST_SUITE_END()

