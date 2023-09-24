# Regular expression assertions

You can assert the contents of strings more dynamically with regular expressions using the 
``assertRegex`` function.

Its signatures (with default or custom message) are:
````c++
TestResult assertRegex(const char* regex, std::string actual)
TestResult assertRegex(const char* regex, std::string actual, const char* message)
````

## Usage
You can test with raw string literals like this:

````c++
assertRegex(R"(^Order \d+$)", "Order 12345", "Checking order number regex");
````

## Exception assertions

For exception assertions with regular expression capabilities, please
read the [Exceptions chapter](/assertions/exceptions).