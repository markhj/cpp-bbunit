@page because Using "because"

You can provide additional description on a per-assertion basis,
using ``because``.

## Example

````cpp
it("Tests some integers", [&]() {
    assertEquals<int>(2, someIntValue).because("It must be 2");
    assertEquals<int>(10, someIntValue).because("It must be 10");
});
````

When tests fail, and they are printed to the console, this additional
information will also be present, making it much easier to identify
exactly which assertion failed.
