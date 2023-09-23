# Code structure

It's generally recommended to code your test in a separate program/executable.
It's not required, but recommended.

Furthermore, you could write your ``CMakeLists.txt`` (or similar) such that
the test suite only compiles when a given environment variable is true.
