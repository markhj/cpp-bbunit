![](https://res.cloudinary.com/drfztvfdh/image/upload/v1720248334/Github/cpp-bbunit2_ecwel6.jpg)

![GitHub Tag](https://img.shields.io/github/v/tag/markhj/cpp-bbunit?label=version)
![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/markhj/cpp-bbunit/doxygen.yml?label=docs)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/f3ae939dbe5c4b1f894a14e00ec60b56)](https://app.codacy.com/gh/markhj/cpp-bbunit/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?label=license)

**C++ BBUnit** is a light-weight header-only unit testing library, inspired
by JUnit and PHPUnit.

## 🌿 Welcome to BBUnit 2!

**_C++ BBUnit 2_** presents a total overhaul of the library, bringing not
only new features, but also a safer and more flexible way of testing.

BBUnit offers:

- **Header-only library**: Very easy to integrate in your project
- **Compare almost any data type**: Make assertions on any data type that has comparable properties
- **When size matters**: Assert sizes of any data type which offers ``size`` method (vector, map, array, etc.)
- **Exceptional exceptions**: Assert that exceptions are thrown, and which
- **^regexp?$**: Assert regular expression patterns on string-types
- **Safer test environment**: Unintended behaviors are caught and managed, and won't crash the test suite
- **Better scoping**: It's much easier to know which test cases are failing

On the code-level, there's greater decoupling between test suite components, and
a much improved structure.

## 🍃 Version support

| Version line  | Support  |
|---------------|----------|
| ``2.0+``      | On-going |
| ``0.x``, ``1.x`` | End of life November 2024 | 

## 📌 Requirements

-   C++20 or higher
-   CMake 3.28 or higher

## 📘 Documentation

The documentation and tutorials are available here:
[C++ BBUnit on GitHub Pages](https://markhj.github.io/cpp-bbunit)
