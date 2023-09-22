#include "../lib/bbunit/bbunit.hpp"

class MyTest : public BBUnit::TestCase {
protected:
    void test() override
    {
        setMode(BBUnit::TestMode::SelfTest);

        numbers();
        greaterThan();
        lessThan();
        bools();
        constChar();
        strings();
        countAndEmpty();
    }

    void greaterThan()
    {
        assertTrue(assertGreaterThan(2, 5, "5 is greater than 2"));
        assertFalse(assertGreaterThan(5, 5, "5 is not greater than 5"));
        assertFalse(assertGreaterThan(8, 5, "5 is not greater than 8"));

        assertTrue(assertGreaterThanOrEqual(2, 5, "5 is greater than or equal to 2"));
        assertTrue(assertGreaterThanOrEqual(5, 5, "5 is greater than or equal to 5"));
        assertFalse(assertGreaterThanOrEqual(8, 5, "5 is NOT greater than or equal to 8"));
    }

    void lessThan()
    {
        assertFalse(assertLessThan(2, 5, "5 is less than 2"));
        assertFalse(assertLessThan(5, 5, "5 is not less than 5"));
        assertTrue(assertLessThan(8, 5, "5 is not less than 8"));

        assertFalse(assertLessThanOrEqual(2, 5, "5 is less than or equal to 2"));
        assertTrue(assertLessThanOrEqual(5, 5, "5 is less than or equal to 5"));
        assertTrue(assertLessThanOrEqual(8, 5, "5 is NOT less than or equal to 8"));
    }

    void numbers()
    {
        assertFalse(assertEquals(2, 5, "Must be 2 (integer)"));
        assertTrue(assertEquals(5, 5, "Must be 5 (integer)"));

        assertFalse(assertEquals(2.0f, 5.0f, "Must be 2 (float)"));
        assertTrue(assertEquals(5.0f, 5.0f, "Must be 5 (float)"));
    }

    void bools()
    {
        assertTrue(assertTrue(true, "assertTrue of true"));
        assertFalse(assertTrue(false, "assertTrue of false"));
        assertFalse(assertFalse(true, "assertFalse of false"));
        assertTrue(assertFalse(false, "assertFalse of true"));

        assertFalse(assertEquals(false, true, "assertEquals of false"));
        assertTrue(assertEquals(true, true, "assertEquals of true"));
    }

    void constChar()
    {
        assertFalse(assertEquals("See you later, world", "Hello world", "Different const chars"));
        assertTrue(assertEquals("Hello world", "Hello world", "Identical const chars"));
    }

    void strings()
    {
        std::string a = "Hello world";
        std::string b = "See you later, world";

        assertFalse(assertEquals(b, a, "Different strings"));
        assertTrue(assertEquals(a, a, "Identical strings"));
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
};

int main()
{
    BBUnit::Dispatcher::dispatch(MyTest());

    return 0;
}
