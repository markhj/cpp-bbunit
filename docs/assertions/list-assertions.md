# List of assertions

Below you can find a list of all method signatures used to make 
assertions in a test case.

The list contains a headline with the method name, and then
you'll see a code block with the different overloaded signatures.

## ``assertEquals``

Test passes if the two values are an exact match.

````c++
TestResult assertEquals(int expected, int actual, const char *message)
TestResult assertEquals(double expected, double actual, const char *message)
TestResult assertEquals(bool expected, bool actual, const char *message)
TestResult assertEquals(const char *expected, const char *actual, const char *message)
TestResult assertEquals(const std::string& expected, const std::string& actual, const char *message)
````

### Examples
````c++
assertEquals(5, 2, "Comparing integers");
assertEquals(5.0f, 5.0f, "Comparing floats");
assertEquals("foo", "bar", "Comparing const chars");
````

## ``assertGreaterThan``
Test passes if the input value is greater than ``compare``

````c++
TestResult assertGreaterThan(int compare, int actual, const char *message)
TestResult assertGreaterThan(double compare, double actual, const char *message)
````

## ``assertGreaterThanOrEqual``
Test passes if the input value is greater than or equal to ``compare``

````c++
TestResult assertGreaterThanOrEqual(int compare, int actual, const char *message)
TestResult assertGreaterThanOrEqual(double compare, double actual, const char *message)
````

## ``assertLessThan``
Test passes if the input value is less than ``compare``

````c++
TestResult assertLessThan(int compare, int actual, const char *message)
TestResult assertLessThan(double compare, double actual, const char *message)
````

## ``assertLessThanOrEqual``
Test passes if the input value is less than or equal to ``compare``

````c++
TestResult assertLessThanOrEqual(int compare, int actual, const char *message)
TestResult assertLessThanOrEqual(double compare, double actual, const char *message)
````

## ``assertTrue`` and ``assertFalse``
Short-hander for boolean comparisons.
Test passes in ``assertTrue`` if the input value evaluates to true.
Similarly, ``assertFalse`` passes if the input evaluates to false.

````c++
TestResult assertTrue(bool actual, const char* message)
TestResult assertFalse(bool actual, const char* message)
````

## ``assertContains`` and ``assertContainsCI``
Test passes if ``actual`` contains ``contains`` at any position within it.

### Case-sensitive variant
````c++
TestResult assertContains(const char *contains, const char* actual, const char* message)
TestResult assertContains(const char *contains, const std::string& actual, const char* message)
````

### Case-insensitive variant
````c++
TestResult assertContainsCI(const char *contains, const char* actual, const char* message)
TestResult assertContainsCI(const char *contains, const std::string& actual, const char* message)
````

## ``assertCount``
Test passes if the number of items in a vector or map matches ``expected``.

````c++
TestResult assertCount(unsigned int expected, std::vector<T> vector, const char* message)
TestResult assertCount(unsigned int expected, std::map<K, V> map, const char* message)
````

## ``assertEmpty``
Short-hand related to ``assertCount`` which passes if a vector or map has no items.

````c++
TestResult assertEmpty(std::vector<T> vector, const char* message)
TestResult assertEmpty(std::map<K, V> map, const char* message)
````