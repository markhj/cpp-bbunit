#ifndef BBUNIT_TEST_STRINGS_HPP
#define BBUNIT_TEST_STRINGS_HPP

#include "bbunit.hpp"

class StringsTest : public BBUnit::TestCase {
public:
    StringsTest() {
        setMode(BBUnit::TestMode::SelfTest);
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

};

#endif
