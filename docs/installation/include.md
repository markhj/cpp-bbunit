# Install as part of project

The library is designed as a single header-only file for fast and easy use.

Copy the file ``lib/bbunit/bbunit.hpp`` to your project.

Depending on the location of the file, you may have to update your ``CMakeLists.txt`` (if you use CMake)
to reflect the include directory of where you added the ``bbunit.hpp`` file.

Now, include with:
````c++
#include "bbunit/bbunit.hpp"
````
Or, depending on how you structured the inclusion and CMake, maybe just:
````c++
#include "bbunit.hpp"
````