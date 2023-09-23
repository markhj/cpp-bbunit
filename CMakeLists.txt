# This CMake file is used to compile the self-test of the library

cmake_minimum_required(VERSION 3.26)

project(bbunit_test)

set(CMAKE_CXX_STANDARD 20)

add_executable(bbunit_test tests/tests.cpp)
