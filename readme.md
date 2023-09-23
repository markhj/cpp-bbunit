# C++ BBUnit

**C++ BBUnit** is a small header-only unit testing library, inspired
by PHPUnit.

To learn everything about installing and operating the library, check the documentation at: https://cpp-bbunit.readthedocs.io

## Don't use in production :guardsman:

This library is very early in its development, and it's **_not_** recommended to
use it in production or important code.

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

## Structuring test code :wrench:
It's generally recommended to code your test in a separate program/executable.
It's not required, but recommended.

Furthermore, you could write your ``CMakeLists.txt`` (or similar) such that
the test suite only compiles when a given environment variable is true.

## CMakeLists.txt :page_with_curl:
The included ``CMakeLists.txt`` compiles an executable, which 
tests the library itself.
