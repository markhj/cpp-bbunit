#include "../lib/bbunit/bbunit.hpp"

class CustomException : public std::exception {
public:
    CustomException() : msg("Default message") {}

    explicit CustomException(const char *message) : msg(message) { }

    [[nodiscard]] const char* what() const noexcept override {
        return msg;
    }
private:
    const char *msg;
};

class MyTest : public BBUnit::TestCase {
public:
    MyTest()
    {
        setMode(BBUnit::TestMode::SelfTest);
    }

    void numbers()
    {
        assertFalse(assertEquals(2, 5));
        assertTrue(assertEquals(5, 5));

        assertFalse(assertEquals(2.0f, 5.0f));
        assertTrue(assertEquals(5.0f, 5.0f));
    }

    void greaterThan()
    {
        assertTrue(assertGreaterThan(2, 5));
        assertFalse(assertGreaterThan(5, 5));
        assertFalse(assertGreaterThan(8, 5));

        assertTrue(assertGreaterThan(2.0, 5.0));
        assertFalse(assertGreaterThan(5.0, 5.0));
        assertFalse(assertGreaterThan(8.0, 5.0));

        assertTrue(assertGreaterThanOrEqual(2, 5));
        assertTrue(assertGreaterThanOrEqual(5, 5));
        assertFalse(assertGreaterThanOrEqual(8, 5));

        assertTrue(assertGreaterThanOrEqual(2.0, 5.0));
        assertTrue(assertGreaterThanOrEqual(5.0, 5.0));
        assertFalse(assertGreaterThanOrEqual(8.0, 5.0));
    }

    void lessThan()
    {
        assertFalse(assertLessThan(2, 5));
        assertFalse(assertLessThan(5, 5));
        assertTrue(assertLessThan(8, 5));

        assertFalse(assertLessThan(2.0, 5.0));
        assertFalse(assertLessThan(5.0, 5.0));
        assertTrue(assertLessThan(8.0, 5.0));

        assertFalse(assertLessThanOrEqual(2, 5));
        assertTrue(assertLessThanOrEqual(5, 5));
        assertTrue(assertLessThanOrEqual(8, 5));

        assertFalse(assertLessThanOrEqual(2.0, 5.0));
        assertTrue(assertLessThanOrEqual(5.0, 5.0));
        assertTrue(assertLessThanOrEqual(8.0, 5.0));
    }

    void bools()
    {
        assertTrue(assertTrue(true));
        assertFalse(assertTrue(false));
        assertFalse(assertFalse(true));
        assertTrue(assertFalse(false));

        assertFalse(assertEquals(false, true));
        assertTrue(assertEquals(true, true));
    }

    void constChar()
    {
        assertFalse(assertEquals("See you later, world", "Hello world"));
        assertTrue(assertEquals("Hello world", "Hello world"));
    }

    void strings()
    {
        std::string a = "Hello world";
        std::string b = "See you later, world";

        assertFalse(assertEquals(b, a));
        assertTrue(assertEquals(a, a));
    }

    void contains()
    {
        std::string str = "Hello world";

        assertTrue(assertContains("world", "Hello world", "Contains 'Hello world'"));
        assertTrue(assertContains("world", str, "Contains 'Hello world'"));

        assertFalse(assertContains("nothing", "Hello world", "Contains 'Hello world'"));
        assertFalse(assertContains("nothing", str, "Contains 'Hello world'"));
    }

    void containsCI()
    {
        std::string str = "Hello World";

        assertTrue(assertContainsCI("WORLD", "Hello World", "Contains 'Hello World' (const char)"));
        assertTrue(assertContainsCI("WORLD", str, "Contains 'Hello World' (string)"));

        assertFalse(assertContainsCI("nothing", "Hello world", "Does not contain 'Hello world'"));
        assertFalse(assertContainsCI("nothing", str, "Does not contain 'Hello world'"));
    }

    void countAndEmpty()
    {
        std::map<int, const char*> map = { {0, "A"}, {1, "B"}, {2, "C"} };
        std::map<int, int> emptyMap = {};
        std::vector<int> emptyVec = {};
        std::vector<int> vec = { 1, 2, 3 };

        assertFalse(assertCount(2, vec, "Vector with 2 elements"));
        assertTrue(assertCount(3, vec, "Vector with 3 elements"));

        assertFalse(assertCount(2, map, "Map with 2 elements"));
        assertTrue(assertCount(3, map, "Map with 3 elements"));

        assertTrue(assertEmpty(emptyMap, "assertEmpty on empty map"));
        assertTrue(assertEmpty(emptyVec, "assertEmpty on empty vector"));
        assertFalse(assertEmpty(map, "assertEmpty on non-empty map"));
        assertFalse(assertEmpty(vec, "assertEmpty on non-empty vector"));
    }

    void exceptions()
    {
        assertTrue(assertException([]() {
            throw std::exception();
        }, "assertException when exception is thrown"));

        assertFalse(assertException([]() {
        }, "assertException when no exception thrown"));

        assertTrue(assertException([]() {
            throw std::runtime_error("Some error");
        }, "assertException when runtime_error is thrown"));

        assertTrue(assertException<CustomException>([]() {
            throw CustomException();
            }, "CustomException thrown and expected"));

        assertFalse(assertException<CustomException>([]() {
            throw std::exception();
        }, "Normal exception thrown, but CustomException expected"));
    }

    void exceptionMessages()
    {
        // Const char
        assertTrue(assertExceptionMessage("Some error", []() {
            throw std::runtime_error("Some error");
        }, "assertExceptionMessage with correct message (const char)"));

        assertFalse(assertExceptionMessage("Incorrect", []() {
            throw std::runtime_error("Some error");
        }, "assertExceptionMessage with wrong message (const char)"));

        // std::string
        assertTrue(assertExceptionMessage(std::string("Hello"), []() {
            throw std::runtime_error("Hello");
        }, "assertExceptionMessage with correct message (string)"));

        assertFalse(assertExceptionMessage(std::string("Incorrect"), []() {
            throw std::runtime_error("Hello");
        }, "assertExceptionMessage with wrong message (string)"));

        // By type (const char)
        assertTrue(assertExceptionMessage<CustomException>("Some error", []() {
            throw CustomException("Some error");
        }, "assertExceptionMessage with correct message (const char)"));

        assertFalse(assertExceptionMessage<CustomException>("Some error", []() {
            throw std::runtime_error("Some error");
        }, "assertExceptionMessage with correct message but wrong type (const char)"));

        // By type (string)
        assertTrue(assertExceptionMessage<CustomException>(std::string("Some error"), []() {
            throw CustomException("Some error");
        }, "assertExceptionMessage with correct message (string)"));

        assertFalse(assertExceptionMessage<CustomException>(std::string("Some error"), []() {
            throw std::runtime_error("Some error");
        }, "assertExceptionMessage with correct message but wrong type (string)"));

        assertFalse(assertExceptionMessage<CustomException>(std::string("False message"), []() {
            throw CustomException("Some error");
        }, "assertExceptionMessage with right type, but wrong content (string)"));
    }

    void exceptionMessageContainsConstChar()
    {
        assertTrue(assertExceptionMessageContains("world", []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionMessageContains with correct content (const char)"));

        assertFalse(assertExceptionMessageContains("Incorrect", []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionMessageContains with wrong content (const char)"));

        assertTrue(assertExceptionMessageContainsCI("WORLD", []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionMessageContains with correct content (const char)"));

        assertFalse(assertExceptionMessageContainsCI("INCORRECT", []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionMessageContains with wrong content (const char)"));

        assertTrue(assertExceptionMessageContains<std::runtime_error>("world", []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionMessageContains with correct content (const char)"));

        assertFalse(assertExceptionMessageContains<CustomException>("world", []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionOfTypeContains with right content but wrong class (const char)"));

        assertTrue(assertExceptionMessageContainsCI<std::runtime_error>("WORLD", []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionMessageContains with correct content (const char)"));

        assertFalse(assertExceptionMessageContainsCI<CustomException>("WORLD", []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionOfTypeContains with right content but wrong class (const char)"));
    }

    void exceptionMessageContainsString()
    {
        assertTrue(assertExceptionMessageContains(std::string("world"), []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionMessageContains with correct content (const char)"));

        assertFalse(assertExceptionMessageContains(std::string("Incorrect"), []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionMessageContains with wrong content (const char)"));

        assertTrue(assertExceptionMessageContains<std::runtime_error>(std::string("world"), []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionMessageContains with correct content (const char)"));

        assertFalse(assertExceptionMessageContains<CustomException>(std::string("world"), []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionOfTypeContains with right content but wrong class (const char)"));
    }

    void regex()
    {
        assertTrue(
                assertRegex(R"(^\d+$)", "12345", "Valid regex")
                );

        assertFalse(
                assertRegex(R"(^\d+$)", "abcdef", "Invalid regex")
        );

        assertTrue(
                assertRegex(R"(^\d+$)", std::string("12345"), "Valid regex")
        );
    }

    void regexExceptions()
    {
        assertTrue(
                assertExceptionMessageRegex(R"(^Hello \d+$)", []() {
                    throw std::runtime_error("Hello 123");
                }, "Exception with valid regex")
                );

        assertFalse(
                assertExceptionMessageRegex(R"(^Hello \d+$)", []() {
                    throw std::runtime_error("Hello world");
                }, "Exception with invalid regex")
        );

        assertTrue(
                assertExceptionMessageRegex<CustomException>(R"(^Hello \d+$)", []() {
                    throw CustomException("Hello 123");
                }, "Exception with valid regex")
        );

        assertFalse(
                assertExceptionMessageRegex<CustomException>(R"(^Hello \d+$)", []() {
                    throw std::runtime_error("Hello 123");
                }, "Exception with valid regex")
        );
    }
};

int main()
{
    BBUnit::TestSuite<MyTest> numbers(&MyTest::numbers,
        &MyTest::greaterThan,
        &MyTest::lessThan);

    BBUnit::TestSuite<MyTest> strings(&MyTest::constChar,
        &MyTest::strings,
        &MyTest::contains,
        &MyTest::containsCI);

    BBUnit::TestSuite<MyTest> regex(&MyTest::regex);

    BBUnit::TestSuite<MyTest> bools(&MyTest::bools);

    BBUnit::TestSuite<MyTest> sets(&MyTest::countAndEmpty);

    BBUnit::TestSuite<MyTest> exceptions(&MyTest::exceptions,
        &MyTest::exceptionMessages,
        &MyTest::exceptionMessageContainsConstChar,
        &MyTest::exceptionMessageContainsString,
        &MyTest::regexExceptions);

    BBUnit::TestRunner().run(numbers,
                             strings,
                             bools,
                             sets,
                             exceptions,
                             regex);

    return 0;
}
