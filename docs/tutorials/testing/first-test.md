@page first-test Your first test

When you have installed C++ BBUnit, you should be able to include it
using:

````cpp
#include <bbunit/bbunit.hpp>
````

If this doesn't work, you need to review your installation.

## First test

A very basic test case looks like this:

````cpp
class Mytest : public BBUnit::TestCase {
    void test() override {
        
    }
};
````

Every child of ``TestCase`` must define the ``test`` method.
This is where you add all your assertions.

Let's zoom in on this method, and make a simple assertion.

````cpp
void test() override {
    it("Tests that two integers match", [&]() {
        assertEquals<int>(10, 10);
    });
}
````

Okay, so there's a bit to unpack here.

First, notice the use of ``it``. This method groups one or  several
assertions together.

Within this scope, we perform a single assertion: We check if
two integers, 10 and 10, are equal. And since they are, the test will
pass.

The idea is of course that you replace one of the hard-coded value
with something your program has calculated. For example:

````cpp
void test() override {
    it("Check when customer's bank balance is positive", [&]() {
        // Set up scenario which creates a positive bank balance
        
        assertEquals<float>(500.0, calculateBankBalance());
    });
}
````

### Other types

When you look at ``assertEquals<T>`` it means you can compare a wide
array of types. You can also compare booleans, strings, other numeric types,
enumerators, etc.

## Run test

Next step is to run the test. For this, we use the BBUnit::TestRunner.

````cpp
std::vector<std::shared_ptr<TestCase>> testCases = {
        std::make_shared<MyTest>(MyTest())
};

TestResults results = TestRunner::run(testCases);
````

``TestRunner`` evaluates all the tests, and gathers the results in
BBUnit::TestResults.

## Printing results

That's all great and dandy, but how do you see the results?

For this, you can use the aptly named BBUnit::Utilities::Printer.

Start by including it:

````cpp
#include <bbunit/utilities/printer.hpp>
````

And then call:

````cpp
BBUnit::Utilities::Printer::print(results, {});
````

When you run the program, you should see the results printed
to the console.
