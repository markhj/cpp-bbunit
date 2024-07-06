# C++ BBUnit
# CMake for self-testing executable

cmake_minimum_required(VERSION 3.28)
project(cpp_bbunit)

# Due to use of std::format and concepts, C++20 is required.
set(CMAKE_CXX_STANDARD 20)

# Make "include" directory visible
include_directories(include)

# Create the "tests" executable
add_executable(tests tests/main.cpp)
