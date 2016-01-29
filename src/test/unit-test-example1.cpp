
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

#define BOOST_TEST_MODULE ExampleTestModule

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

// The two relevant Boost namespaces for the unit test framework are:
using namespace boost;
using namespace boost::unit_test;

// Provide a name for our suite of tests. This statement is used to bracket
// our test cases.
BOOST_AUTO_TEST_SUITE(ExampleTestSuite)

// The structure below allows us to pass a test initialization object to
// each test case. Note the use of struct to default all methods and member
// variables to public access.
struct TestFixture
{
    TestFixture() : i( 0 ) {
        // Put test initialization here, the constructor will be called
        // prior to the execution of each test case
        printf("Initialize test\n");
    }
    ~TestFixture() {
        // Put test cleanup here, the destructor will automatically be
        // invoked at the end of each test case.
        printf("Cleanup test\n");
    }
    // Public test fixture variables are automatically available to all test
    // cases. Don’t forget to initialize these variables in the constructors
    // to avoid initialized variable errors.
    int i;
};

// Define a test case. The first argument specifies the name of the test.
// Take some care in naming your tests. Do not reuse names or accidentally use
// the same name for a test as specified for the module test suite name.
//
// The second argument provides a test build-up/tear-down object that is
// responsible for creating and destroying any resources needed by the
// unit test
BOOST_FIXTURE_TEST_CASE(BoostWarnTest, TestFixture)
{
    // Verify i is not 0
    BOOST_REQUIRE_NO_THROW(i = 4/i);
    // Verify i is equal to 3
    BOOST_CHECK(i == 3);

}

// Define a test case. The first argument specifies the name of the test.
// Take some care in naming your tests. Do not reuse names or accidentally use
// the same name for a test as specified for the module test suite name.
//
// The second argument provides a test build-up/tear-down object that is
// responsible for creating and destroying any resources needed by the
// unit test
BOOST_FIXTURE_TEST_CASE(BoostWarnTest2, TestFixture)
{
    // Provide a test predicate (i.e. a conditional statement) that evaluates
    // to true to allow the test to pass and will not issue a warning.
    BOOST_WARN(true);

    // Provide a test predicate (i.e. a conditional statement) that evaluates
    // to false to force the test to fail and will issue a warning.
    BOOST_WARN(false);
}

BOOST_FIXTURE_TEST_CASE(BoostCheckTest, TestFixture)
{
    // Provide a test predicate (i.e. a conditional statement) that evaluates
    // to true to allow the test to pass and will not indicate a failed test.
    BOOST_CHECK(true);

    // Provide a test predicate (i.e. a conditional statement) that evaluates
    // to false to force the test to fail and will indicate a test failure.
    BOOST_CHECK(false);
}

BOOST_FIXTURE_TEST_CASE(BoostRequireTest, TestFixture)
{
    // Provide a test predicate (i.e. a conditional statement) that evaluates
    // to true to allow the test to pass and will not indicate a fatal test error.
    BOOST_REQUIRE(true);

    // Provide a test predicate (i.e. a conditional statement) that evaluates
    // to false to force the test to fail and will indicate a fatal test error.
    BOOST_REQUIRE(false);
}

BOOST_FIXTURE_TEST_CASE(BoostWarnMessageTest, TestFixture)
{
    // Provide a test predicate (i.e. a conditional statement) that evaluates
    // to true to allow the test to pass and will not issue a warning with
    // message.
    BOOST_WARN_MESSAGE(true, "This should pass");

    // Provide a test predicate (i.e. a conditional statement) that evaluates
    // to false to force the test to fail and will issue a warning with message.
    BOOST_WARN_MESSAGE(false, "This should fail");
}

BOOST_FIXTURE_TEST_CASE(BoostCheckMessageTest, TestFixture)
{
    // Provide a test predicate (i.e. a conditional statement) that evaluates
    // to true to allow the test to pass and not will indicate a failed test
    // with message.
    BOOST_CHECK_MESSAGE(true, "This should pass");

    // Provide a test predicate (i.e. a conditional statement) that evaluates
    // to false to force the test to fail and will indicate a test failure
    // with message.
    BOOST_CHECK_MESSAGE(false, "This should fail");
}

BOOST_FIXTURE_TEST_CASE(BoostRequireMessageTest, TestFixture)
{
    // Provide a test predicate (i.e. a conditional statement) that evaluates
    // to true to allow the test to pass and will not indicate a fatal test
    // error with message.
    BOOST_REQUIRE_MESSAGE(true, "This should pass");

    // Provide a test predicate (i.e. a conditional statement) that evaluates
    // to false to force the test to fail and will indicate a fatal test error
    // with message.
    BOOST_REQUIRE_MESSAGE(false, "This should fail");
}

BOOST_FIXTURE_TEST_CASE(BoostWarnThrowTest, TestFixture)
{
    // Provide a statement that is expected to throw with a specific exception.
    // Provide the expected exception as the second argument. Since the
    // exception types match, the test will pass and will not issue a warning.
    BOOST_WARN_THROW(
        throw std::range_error("This should pass"),
        std::range_error);

    // Provide a statement that is expected to throw with a specific exception.
    // Provide the expected exception as the second argument. Since the
    // exception types do not match, the test will not pass and will issue a
    // warning.
    BOOST_WARN_THROW(
        throw std::range_error("This should fail"),
        std::underflow_error);
}

BOOST_FIXTURE_TEST_CASE(BoostCheckThrowTest, TestFixture)
{
    // Provide a statement that is expected to throw with a specific
    // exception. Provide the expected exception as the second argument.
    // Since the exception types match, the test will pass and will
    // not issue a test failure.
    BOOST_CHECK_THROW(
        throw std::range_error("This should pass"),
        std::range_error);

    // Provide a statement that is expected to throw with a specific
    // exception. Provide the expected exception as the second argument.
    // Since the exception types do not match, the test will not pass and
    // will issue a test failure.
    BOOST_CHECK_THROW(
        throw std::range_error("This should fail"),
        std::underflow_error);
}

BOOST_FIXTURE_TEST_CASE(BoostRequireThrowTest, TestFixture)
{
    // Provide a statement that is expected to throw with a specific
    // exception. Provide the expected exception as the second argument.
    // Since the exception types match, the test will pass and will not
    // issue a fatal test error.
    BOOST_REQUIRE_THROW(
        throw std::range_error("This should pass"),
        std::range_error);

    // Provide a statement that is expected to throw with a specific
    // exception. Provide the expected exception as the second argument.
    // Since the exception types do not match, the test will not pass
    // and will issue a fatal test error.
    BOOST_REQUIRE_THROW(
        throw std::range_error("This should fail"),
        std::underflow_error);
}

BOOST_FIXTURE_TEST_CASE(BoostWarnNoThrowTest, TestFixture)
{
    // Provide a statement that is not expected to throw. If the statement
    // does not throw, the test will pass and not issue a warning.
    BOOST_WARN_NO_THROW(i = 1);

    // Provide a statement that is expected to not throw. If the statement
    // does throw, the test will fail and issue a warning.
    BOOST_WARN_NO_THROW( throw std::range_error("This should fail"));
}

BOOST_FIXTURE_TEST_CASE(BoostCheckNoThrowTest, TestFixture)
{
    // Provide a statement that is not expected to throw. If the statement
    // does not throw, the test will pass and not issue a test failure.
    BOOST_CHECK_NO_THROW(i = 2);

    // Provide a statement that is expected to not throw. If the statement
    // does throw, the test will fail and issue a test failure.
    BOOST_CHECK_NO_THROW( throw std::range_error("This should fail"));
}

BOOST_FIXTURE_TEST_CASE(BoostRequireNoThrowTest, TestFixture)
{
    // Provide a statement that is not expected to throw. If the statement
    // does not throw, the test will pass and not issue a test failure.
    BOOST_REQUIRE_NO_THROW(i = 3);

    // Provide a statement that is expected to not throw. If the statement
    // does throw, the test will fail and issue a test failure.
    BOOST_REQUIRE_NO_THROW( throw std::range_error("This should fail"));
}

// This must match the BOOST_AUTO_TEST_SUITE(ExampleTestSuite) statement
// above and is used to bracket our test cases.

BOOST_AUTO_TEST_SUITE_END()

