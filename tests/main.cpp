#include "../lib/bbunit/bbunit.hpp"
#include "comparison.hpp"
#include "equals.hpp"
#include "strings.hpp"
#include "collections.hpp"
#include "exceptions.hpp"

int main()
{
    BBUnit::TestSuite<EqualsTest> equals(&EqualsTest::numbers,
        &EqualsTest::constChar,
        &EqualsTest::strings,
        &EqualsTest::bools);

    BBUnit::TestSuite<ComparisonOperatorTest> comparison(
        &ComparisonOperatorTest::greaterThan,
        &ComparisonOperatorTest::lessThan);

    BBUnit::TestSuite<StringsTest> strings(
        &StringsTest::contains,
        &StringsTest::containsCI,
        &StringsTest::regex);

    BBUnit::TestSuite<CollectionsTest> collections(&CollectionsTest::count,
        &CollectionsTest::empty);

    BBUnit::TestSuite<ExceptionsTest> exceptions(&ExceptionsTest::exceptions,
        &ExceptionsTest::exceptionsOfType,
        &ExceptionsTest::exceptionMessages,
        &ExceptionsTest::exceptionMessageContainsConstChar,
        &ExceptionsTest::exceptionMessageContainsString,
        &ExceptionsTest::regexExceptions);

    BBUnit::TestRunner().run(equals,
                             comparison,
                             strings,
                             collections,
                             exceptions);

    return 0;
}
