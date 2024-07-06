/**
 * C++ BBUnit: Self-test
 *
 * In this file, C++ BBUnit will be testing itself :-)
 *
 * A set of features are built into BBUnit, making it possible or the library
 * to test itself.
 */

#include <bbunit/bbunit.hpp>
#include <bbunit/utilities/printer.hpp>

#include "./bbunit-test.cpp"

using namespace BBUnit;
using namespace BBUnit::Tests;

int main() {
    TestResults results = TestRunner::run({std::make_shared<BBUnitTest>(BBUnitTest())});

    Utilities::Printer::print(results, {});
}

