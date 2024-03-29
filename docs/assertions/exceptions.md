# Assert exceptions

With C++ BBUnit you can also test if exceptions are thrown.

All ``assertException*`` methods take a lambda function in which it's check if
an exception is thrown (or not). Optionally they take template arguments,
if you want to verify that a specific exception class is thrown.

## Default messages

Like with all other assertion methods, the ``message`` parameter is optional.
All functions demonstrated below can be used with or without it.

If you don't specify a message the library generates a standardized one.

## ``assertException``
Assert that an exception is thrown (regardless of what message is given).

````c++
assertException([]() {
    throw std::exception();
}, "Message")
````

This test passes if an exception is thrown inside the lambda function.

You verify that a specific exception class is thrown with a template argument:

````c++
assertException<std::runtime_error>([]() {
    throw std::exception();
}, "Message")
````

This test will fail, because ``std::runtime_error`` is expected, but a regular `´std::exception` is thrown.

## ``assertExceptionMessage``
You can verify that an exception (regardless of type) has an exact exception message with:

````c++
assertExceptionMessage("Expected message", []() {
    throw std::runtime_error("Expected message");
}, "Message")
````

The message must be exact in length and case-sensitivity.

If you expect a specific class type, you can do it like this:

````c++
assertExceptionMessage<std::runtime_error>("Expected message", []() {
    throw std::runtime_error("Expected message");
}, "Message")
````

To check the message the ``what()`` method of an exception is called.

## ``assertExceptionMessageContains``

In essence, similar to ``assertExceptionMessage``, but checks if the error message 
contains the expected message.

````c++
assertExceptionMessageContains("world", []() {
    throw std::runtime_error("Hello world");
}, "Message")
````

Like the other methods, you can call with a template argument:

````c++
assertExceptionMessageContains<std::runtime_error>("world", []() {
    throw std::runtime_error("Hello world");
}, "Message")
````

## ``assertExceptionMessageContainsCI``

The case-insensitive variant of ``assertExceptionMessageContains``.

## ``assertExceptionMessageRegex``

You can assert that the exception message matches a regular expression pattern using
``assertExceptionMessageRegex``.

For example:

````c++
assertRegex(R"(^Error no. \d+ was encountered$)", errorMessage, "Test general error message");
````