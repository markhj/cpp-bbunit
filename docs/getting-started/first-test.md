# First test

It's assumed you have installed the library and know how to include it.

## Structure
C++ BBUnit follows in the footsteps of many other frameworks.

- You have a series of test cases
- Test cases are added to test suites
- A test runner executes the tests contained in a suite and provides a summary

## Example
Let's review a practical example.

First, we create a ``TestCase``:
````c++
class MyTest : public BBUnit::TestCase {
public:
    void myFirstTest()
    {
        assertEquals(5, 10, "Compare integers (will fail)");
        assertEquals(10, 10, "Compare integers (will pass)");
    }
};
````

Keep in mind that methods you'd like to test must be public.

Next, we create a ``TestSuite``. A test suite is linked to a single class (i.e.
a test suite cannot test several ``TestCase`` classes).

````c++
int main()
{
    BBUnit::TestSuite<MyTest> mySuite(&MyTest::myFirstTest);
    
    return 0;
}
````

There a few things to unpack here.
As you see, ``TestSuite`` takes a template argument (``MyTest``).
But as important: The methods you'd like to have tested must be added
in the constructor.
The constructor takes a variadic argument, so you can add as many
methods as you need.

````c++
BBUnit::TestSuite<MyTest> mySuite(&MyTest::caseA, &MyTest::caseB, &MyTest::caseC);
````

## Run the test
Once you've set up your test suite, you can run it with the ``TestRunner``.
````c++
BBUnit::TestRunner().run(mySuite);
````

The ``run`` method takes a variadic argument, so you can add as many test suites
as you'd like.

Now when you compile and run the program, it will perform the test(s).