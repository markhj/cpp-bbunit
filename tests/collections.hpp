#ifndef BBUNIT_TEST_COLLECTIONS_HPP
#define BBUNIT_TEST_COLLECTIONS_HPP

#include "bbunit.hpp"

class CollectionsTest : public BBUnit::TestCase {
public:
    CollectionsTest() {
        setMode(BBUnit::TestMode::SelfTest);
    }

    void count()
    {
        std::map<int, const char*> map = { {0, "A"}, {1, "B"}, {2, "C"} };
        std::vector<int> vec = { 1, 2, 3 };

        assertFalse(assertCount(2, vec, "Vector with 2 elements"));
        assertTrue(assertCount(3, vec, "Vector with 3 elements"));

        assertFalse(assertCount(2, map, "Map with 2 elements"));
        assertTrue(assertCount(3, map, "Map with 3 elements"));
    }

    void empty()
    {
        std::map<int, const char*> map = { {0, "A"}, {1, "B"}, {2, "C"} };
        std::map<int, int> emptyMap = {};
        std::vector<int> emptyVec = {};
        std::vector<int> vec = { 1, 2, 3 };

        assertTrue(assertEmpty(emptyMap, "assertEmpty on empty map"));
        assertTrue(assertEmpty(emptyVec, "assertEmpty on empty vector"));
        assertFalse(assertEmpty(map, "assertEmpty on non-empty map"));
        assertFalse(assertEmpty(vec, "assertEmpty on non-empty vector"));
    }

};

#endif
