@page assert-optional Assert std::optional

You can perform tests on ``std::optional`` using the methods
described below. The advantage of using these methods is that you
can safely rely on them not to crash, even if the ``std::optional``
is empty, when attempting to compare it against a value.

To check if an optional is empty, use:

````cpp
std::optional<int> empty;
std::optional<int> hasValue = 10;

assertEmpty(empty);  // true
assertEmpty(empty);  // false
````

You can compare the contents of the ``std::optional`` using
``assertOptional``

````cpp
assertEquals(10, empty); // fails
assertEquals(10, hasValue); // passes
assertEquals(750, hasValue); // fails
````