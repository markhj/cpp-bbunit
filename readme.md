# C++ BBUnit

**C++ BBUnit** is a small header-only unit testing library, inspired
by PHPUnit.

## Don't use in production :guardsman:

This library is very early in its development, and it's **_not_** recommended to
use it in production or important code.

## Setting up :sunny:
The library is designed as a single header-only file for fast and easy use.

Copy the file ``lib/bbunit/bbunit.hpp`` to your project.

Depending on the location of the file, you may have to update your ``CMakeLists.txt`` (if you use CMake)
to reflect the include directory of where you added the ``bbunit.hpp`` file.

Now, include with:
````c++
#include "bbunit/bbunit.hpp"
````
Or, depending on how you structured the inclusion and CMake, maybe just:
````c++
#include "bbunit.hpp"
````

## Usage :hammer:
You can create any number of test classes. There are many strategies,
but typically you might want one test class per implementation class.

````c++
class MyTest : public BBUnit::TestCase {
public:
    void test() override
    {
        assertEquals(10, 5, "Verify that value is 10");
    }
    
};
````

To run the code, use:

````c++
int main()
{
    BBUnit::Dispatcher::dispatch(MyTest());
}
````

You can add multiple test classes:

````c++
int main()
{
    BBUnit::Dispatcher::dispatch(Test1(), Test2(), Test3());
}
````

## All functions :satellite:

### Assert equals
````
assertEquals(int expected, int actual, const char *message)
assertEquals(double expected, double actual, const char *message)
assertEquals(bool expected, bool actual, const char *message)
assertEquals(const std::string& expected, const std::string& actual, const char *message)
````

Checks if there's an exact match between an expectation and an actual value.

### Boolean shorthands
````
assertTrue(bool actual, const char* message)
assertFalse(bool actual, const char* message)
````
Used to directly test if an actual boolean value is true or false.

### Assert count
````
assertCount(unsigned int expected, std::vector<T> vector, const char* message)
assertCount(unsigned int expected, std::map<K, V> map, const char* message)
````
Checks if the number of items in a vector or a map match the expectation.

### Assert empty
````
assertEmpty(std::vector<T> vector, const char* message)
assertEmpty(std::map<K, V> map, const char* message)
````

Shorthand of ``assertCount`` which directly verifies if there are 0 elements.

## Structuring test code :wrench:
It's generally recommended to code your test in a separate program/executable.
It's not required, but recommended.

Furthermore, you could write your ``CMakeLists.txt`` (or similar) such that
the test suite only compiles when a given environment variable is true.

## CMakeLists.txt :page_with_curl:
The included ``CMakeLists.txt`` compiles an executable, which 
tests the library itself.

## Roadmap :blue_car:
- Adding more assertion types, such as "greater than or equals to", etc.

- Tools to test for partial content of strings
- Test for thrown exceptions
- Test for contents of a collection type (vector, map, etc.)
- Improved capabilities to collect cases in suites
- Improved test runner class