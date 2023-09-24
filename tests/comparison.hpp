#ifndef BBUNIT_TEST_COMPARISON_HPP
#define BBUNIT_TEST_COMPARISON_HPP

#include "bbunit.hpp"

class ComparisonOperatorTest : public BBUnit::TestCase {
public:
    ComparisonOperatorTest() {
        setMode(BBUnit::TestMode::SelfTest);
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

};

#endif
