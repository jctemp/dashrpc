cmake_minimum_required(VERSION 3.18)

set(PROJECT "dash_wrapper")
project("${PROJECT}" VERSION 0.1.0 LANGUAGES CXX)

# Global CMake variables are set here
set(CMAKE_CXX_STANDARD              11)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS   ON)

# Options
option(ENABLE_TESTING "Enable a Unit Testing build." ON)

# Cmake modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)

if (ENABLE_TESTING)
    include(CTest)
endif()

# Project
set(EXECUTABLE_NAME "${PROJECT}_Executable")
set(TEST_MAIN       "${PROJECT}_unit_tests")

# Library
set(LIB_DASHRPC "libdashrpc")

# Add conan libraries
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()


# Add subdirectories
add_subdirectory(src)

# Add unit tests
if (ENABLE_TESTING OR ENABLE_COVERAGE)
    enable_testing()
    add_subdirectory(tests)
endif()
