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

        assertFalse(assertCount(2, vec));
        assertTrue(assertCount(3, vec));

        assertFalse(assertCount(2, map));
        assertTrue(assertCount(3, map));
    }

    void empty()
    {
        std::map<int, const char*> map = { {0, "A"}, {1, "B"}, {2, "C"} };
        std::map<int, int> emptyMap = {};
        std::vector<int> emptyVec = {};
        std::vector<int> vec = { 1, 2, 3 };

        assertTrue(assertEmpty(emptyMap));
        assertTrue(assertEmpty(emptyVec));
        assertFalse(assertEmpty(map));
        assertFalse(assertEmpty(vec));
    }

};

#endif
