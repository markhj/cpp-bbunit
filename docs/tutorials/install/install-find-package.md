@page install-submodule Install as submodule

On this page, we're going to look at how you can integrate C++ BBUnit
into your project with CMake's ``find_package`` and Git's submodule system.

## Set up as submodule

Start by setting up the submodule. It's recommended to add the module
in a subdirectory such as ``vendor`` or ``thirdparty``.

````bash
git submodule add https://github.com/markhj/cpp-bbunit <path>
````

Replace ``<path>`` with the directory, including "cpp-bbunit", such as
``vendor/cpp-bbunit``.

````bash
git submodule add https://github.com/markhj/cpp-bbunit vendor/cpp-bbunit
````

### Submodule on new system

When your project is set up on a new machine, you need to run:

````bash
git submodule init
git submodule update
````

## Integrate in ``CMakeLists.txt``

When the submodule is created, you need to add a few lines to your
project's ``CMakeLists.txt`` file:

### Library location

There are two ways to inform CMake where C++ BBUnit is.
You can either set an environment variable called ``CPP_BBUNIT_DIR``.

Alternatively, since this is a submodule, you can specify the location.

````cmake
set(cpp_bbunit_DIR vendor/bbunit/cmake)
````

@note When the library is outside the project scope it is _not_ recommended
to explicitly define the path with ``set``.

### Linking

Now, add:

````cmake
find_package(cpp_bbunit REQUIRED)
````

Assuming your executable is called ``MyApp``, you further
need to link the library like this:

````cmake
target_link_libraries(MyApp PRIVATE cpp_bbunit)
````

## That's it!

After refreshing CMake, you should now be able to use
``#include <bbunit/bbunit.hpp>``.
