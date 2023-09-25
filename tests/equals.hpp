#ifndef BBUNIT_TEST_EQUALS_HPP
#define BBUNIT_TEST_EQUALS_HPP

#include "bbunit.hpp"

class EqualsTest : public BBUnit::TestCase {
public:
    EqualsTest() {
        setMode(BBUnit::TestMode::SelfTest);
    }

    void numbers()
    {
        assertFalse(assertEquals(2, 5));
        assertTrue(assertEquals(5, 5));

        assertFalse(assertEquals(2.0f, 5.0f));
        assertTrue(assertEquals(5.0f, 5.0f));
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

    void bools()
    {
        assertTrue(assertTrue(true));
        assertFalse(assertTrue(false));
        assertFalse(assertFalse(true));
        assertTrue(assertFalse(false));

        assertFalse(assertEquals(false, true));
        assertTrue(assertEquals(true, true));
    }

    void notEquals()
    {
        assertTrue(assertNotEquals(2, 5));
        assertFalse(assertNotEquals(5, 5));

        assertTrue(assertNotEquals(2.0f, 5.0f));
        assertFalse(assertNotEquals(5.0f, 5.0f));

        assertTrue(assertNotEquals(false, true));
        assertFalse(assertNotEquals(true, true));

        assertTrue(assertNotEquals("See you later, world", "Hello world"));
        assertFalse(assertNotEquals("Hello world", "Hello world"));

        std::string a = "Hello world";
        std::string b = "See you later, world";

        assertTrue(assertNotEquals(b, a));
        assertFalse(assertNotEquals(a, a));
    }
};

#endif
