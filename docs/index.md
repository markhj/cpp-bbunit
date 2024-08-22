# Welcome to C++ BBUnit!

**C++ BBUnit** is a header-only unit testing library for C++, inspired by
JUnit and PHPUnit.

The "BB" in "BBUnit" stands for "black box". The reference to _Star Wars_
and _Death Stranding_ is a happy coincidence.

These pages comprise the manual for the library. You can
visit [the repository](https://github.com/markhj/cpp-bbunit) to explore
the code, and get started.

## 🚀 Getting started

To get started, you can read @ref install "how to install", or if you've
already done that, you can @ref tutorials "learn how to write tests".

## 📦 A note on version requirement

C++ BBUnit depends on at least C++20 since version 2.0.
This is due to usage of features like concepts, which are detrimental
to the new structure in the library.

You can use version ``0.x``/``1.x`` for C++17 support, if upgrading isn't
feasible in your case.
