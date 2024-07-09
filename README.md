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

### Keep your app in check!

The more complex a project grows, the higher the risk of a change in one
place, breaking something in another.

This is where unit and feature testing come in, and make your life a lot
easier. Start covering your code, today!

### Header-only library

Very easy to integrate in your project, and no need to deal with building
and linking. Just ``#include`` and you're good to go!

### Compare (almost) any data type

With the introduction of a new template-based assertion system, you
can compare almost any data type. It's as simple as:

````c++
assertEquals<float>(expected, actual);
````

And what about classes? If you find yourself frequently asserting
things about classes, just make sure they have a comparison overloader,
and they can be checked by BBUnit.

### Size, exceptions and regex

But it doesn't stop there. You can also:

- Assert sizes of any data type which offers ``size`` method (vector, map, array, etc.)
- Assert that exceptions are thrown and which type
- Assert regular expression patterns on string-types

### Reporting

The reporting system is more detailed, and miles ahead of the legacy library
in terms of providing information about _where_ the problem arose.

What you want to see, though, is:

![](https://res.cloudinary.com/drfztvfdh/image/upload/v1720542746/image_2024-07-09_183224216_x6z6ny.png)

And not this:

![](https://res.cloudinary.com/drfztvfdh/image/upload/v1720543024/image_2024-07-09_183703424_so4e4s.png)

### Safe environment

Unintended behaviors, mainly exceptions, are caught and managed, without crashing
the test suite.

![](https://res.cloudinary.com/drfztvfdh/image/upload/v1720543161/image_2024-07-09_183919882_cgjwk0.png)

### Improved code

On the code-level, there's greater decoupling between test suite components, and
a much improved structure.

## 🍃 Version support

| Version line     | Support                   |
|------------------|---------------------------|
| ``2.x``          | On-going                  |
| ``0.x``, ``1.x`` | End of life November 2024 | 

## 📌 Requirements

- C++20 or higher
- CMake 3.28 or higher

## 📘 Documentation

The documentation and tutorials are available here:
[C++ BBUnit on GitHub Pages](https://markhj.github.io/cpp-bbunit)
