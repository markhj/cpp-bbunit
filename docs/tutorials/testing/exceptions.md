@page exceptions Testing exceptions

Sometimes you'll want to test if specific exceptions are thrown in
different scenarios.

## Basics

You can test if an exception is thrown, by using ``assertException``.

````cpp
it("Test if std::bad_optional_access is thrown", [&]() {
    assertException<std::bad_optional_access>([&]() {
        std::optional<int>().value();
    });
});
````

Let's break down what happens:

- ``assertException`` takes a template argument, which is the exception
  class which is expected to be thrown.
- The lambda function body executes the code, which is expected to
  trigger the exception.
- For this specific example, ``std::optional<int>().value()``, is just
  a way to trigger the expected exception.

### Behavior

- If no exception is thrown, the case will fail
- If incorrect exception type is thrown, the case will also fail

## Message

Can you can also test if the exception contains a certain message, by
adding a string as the second argument to ``assertException``.

````cpp
it("Test if std::bad_optional_access is thrown", [&]() {
    assertException<std::bad_optional_access>([&]() {
        std::optional<int>().value();
    }, "bad optional access");
});
````

This looks almost identical, but notice the string added as the second
argument.

### Behavior

- If the exception type is correct and the message matches, the test passes
- If the exception type is correct, but the message doesn't match, the test fails
- If the exception is wrong the test fails, no matter if the message is right or wrong

## Technical notes

@note ``assertException`` takes a template can be any class which has
a method named ``what`` (which ``std::exception`` and derivatives do).
