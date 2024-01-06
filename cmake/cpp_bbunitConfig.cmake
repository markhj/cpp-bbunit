include(CMakePackageConfigHelpers)

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../" ABSOLUTE)

add_library(cpp_bbunit
        INTERFACE
        ${PACKAGE_PREFIX_DIR}/lib/bbunit/bbunit.hpp
)

target_include_directories(cpp_bbunit
        INTERFACE
        $<BUILD_INTERFACE:${PACKAGE_PREFIX_DIR}/lib>
        $<INSTALL_INTERFACE:lib>
)

export(TARGETS cpp_bbunit
        FILE ${CMAKE_CURRENT_BINARY_DIR}/cpp-bbunitTargets.cmake)

install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/cpp-bbunitConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/cpp-bbunitConfigVersion.cmake"
        DESTINATION lib/cmake/cpp-bbunit
)
