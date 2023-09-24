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

        assertTrue(assertContains("world", "Hello world"));
        assertTrue(assertContains("world", str));

        assertFalse(assertContains("nothing", "Hello world"));
        assertFalse(assertContains("nothing", str));
    }

    void containsCI()
    {
        std::string str = "Hello World";

        assertTrue(assertContainsCI("WORLD", "Hello World"));
        assertTrue(assertContainsCI("WORLD", str));

        assertFalse(assertContainsCI("nothing", "Hello world"));
        assertFalse(assertContainsCI("nothing", str));
    }

    void regex()
    {
        assertTrue(
                assertRegex(R"(^\d+$)", "12345")
        );

        assertFalse(
                assertRegex(R"(^\d+$)", "abcdef")
        );

        assertTrue(
                assertRegex(R"(^\d+$)", std::string("12345"))
        );
    }

};

#endif
