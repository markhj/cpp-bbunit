# Install with ``find_package``

Since version 0.7 it has been possible to install C++ BBUnit with CMake's
``find_package``.

## Clone repository
Start by cloning the repository to a location on your machine:

``git clone https://github.com/markhj/cpp-bbunit``

## Add environment variable
Create an environment variable which is discoverable by CMake, named
``cpp_bbunit_DIR`` and set its value to the location of the repository
you just cloned.

## Find package
Add this line in your ``CMakeLists.txt``:

````cmake
find_package(cpp_bbunit REQUIRED)
````

Remember to also target the library on your executable
or library. For example:

````cmake
target_link_libraries(my_app PRIVATE cpp_bbunit)
````

Last step is to rebuild your CMake.

## Usage
Include C++ BBUnit at the top of your testcase code with:

````c++
#include <bbunit/bbunit.hpp>
````
