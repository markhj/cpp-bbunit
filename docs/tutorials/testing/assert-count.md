@page assert-count Counting elements

When you want to assert the number of elements in objects like
``std::vector``, ``std::map`` or ``std::array``, you can use
``assertCount``.

@note ``assertCount`` works with any object type which has a method
named ``size`` that returns ``size_t``.

## Example

````cpp
std::vector a = { 1, 2, 3 };

assertCount<std::vector>(3, a);   // True, because there are 3 elements
````

## See also

@ref BBUnit::ProvidesAssertions::assertCount
