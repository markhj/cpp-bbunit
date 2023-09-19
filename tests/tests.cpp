#include "../lib/bbunit/bbunit.hpp"

class MyTest : public BBUnit::TestCase {
protected:
    void test() override
    {
        numbers();
        bools();
        strings();
        countAndEmpty();
    }

    void numbers()
    {
        assertEquals(2, 5, "Must be 2 (integer)");
        assertEquals(5, 5, "Must be 5 (integer)");

        assertEquals(2.0f, 5.0f, "Must be 2 (float)");
        assertEquals(5.0f, 5.0f, "Must be 5 (float)");
    }

    void bools()
    {
        assertTrue(true, "assertTrue of true");
        assertTrue(false, "assertTrue of false");
        assertFalse(true, "assertFalse of false");
        assertFalse(false, "assertFalse of true");

        assertEquals(false, true, "assertEquals of false");
        assertEquals(true, true, "assertEquals of true");
    }

    void strings()
    {
        assertEquals("See you later, world", "Hello world", "Different strings");
        assertEquals("Hello world", "Hello world", "Identical strings");
    }

    void countAndEmpty()
    {
        std::map<int, const char*> map = { {0, "A"}, {1, "B"}, {2, "C"} };
        std::map<int, int> emptyMap = {};
        std::vector<int> emptyVec = {};
        std::vector<int> vec = { 1, 2, 3 };

        assertCount(2, vec, "Vector with 2 elements");
        assertCount(3, vec, "Vector with 3 elements");

        assertCount(2, map, "Map with 2 elements");
        assertCount(3, map, "Map with 3 elements");

        assertEmpty(emptyMap, "assertEmpty on empty map");
        assertEmpty(emptyVec, "assertEmpty on empty vector");
        assertEmpty(map, "assertEmpty on non-empty map");
        assertEmpty(vec, "assertEmpty on non-empty vector");
    }
};

int main()
{
    BBUnit::Dispatcher::dispatch(MyTest());

    return 0;
}
