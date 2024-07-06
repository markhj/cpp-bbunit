@page equals-custom-class assertEquals and custom classes

You can use ``assertEquals`` on any class which has comparison
operator.

This means that you can make it possible for C++ BBUnit to test
entire custom classes.

## Diving in

This would normally not work:

````cpp
class A {
    int x;
};

A one, two;

it ("Tests A", [&]() {
    assertEquals<A>(one, two);    
});
````

Because C++, and by extension BBunit, is not instructed on how to compare them.

However, you can implement an operator overloader, like this:

````cpp
class A {
    int x;
    bool operator==(const A &other) const {
        return x == other.x;
    }
};
````

And now BBUnit will be able to compare two instances of ``A``.
