# This CMake file is used to compile the self-test of the library

cmake_minimum_required(VERSION 3.26)

project(bbunit_test)

set(CMAKE_CXX_STANDARD 20)

include_directories(lib/bbunit)

add_executable(bbunit_test
        lib/bbunit/bbunit.hpp
        tests/main.cpp
        tests/comparison.hpp
        tests/equals.hpp
        tests/strings.hpp
        tests/collections.hpp
        tests/exceptions.hpp)
