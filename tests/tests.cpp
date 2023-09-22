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
        strings();
        countAndEmpty();
    }

    void greaterThan()
    {
        assertTrue(assertGreaterThan(2, 5, "5 is bigger than 2"));
        assertFalse(assertGreaterThan(5, 5, "5 is not bigger than 5"));
        assertFalse(assertGreaterThan(8, 5, "5 is not bigger than 8"));
    }

    void lessThan()
    {

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

    void strings()
    {
        assertFalse(assertEquals("See you later, world", "Hello world", "Different strings"));
        assertTrue(assertEquals("Hello world", "Hello world", "Identical strings"));
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
