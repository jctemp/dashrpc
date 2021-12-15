cmake_minimum_required(VERSION 3.18)

set(PROJECT "dashrpc")
project("${PROJECT}" VERSION 0.2.0 LANGUAGES CXX)

# GLOBAL CMAKE SETTINGS
set(CMAKE_CXX_STANDARD              17)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS   ON)

# OPTIONS
option(ENABLE_TESTING "Enable a Unit Testing build." ON)

# INCLUDES
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# PROJECT FINAL TARGETS
# set(EXECUTABLE_NAME "${PROJECT}_app")
set(TEST_MAIN       "${PROJECT}_unit_tests")

# LIBRARY AND SUBDIRECTORY
set(LIB_DASHPROTOS "libdashprotos")
set(LIB_DASHRPC  "libdashrpc")

add_subdirectory(libdashprotos)
add_subdirectory(libdashrpc)


# UNIT TESTS
if (ENABLE_TESTING)

    include(CTest)

    set(JSON_FILES_DIR "\"${PROJECT_SOURCE_DIR}/tests/jsontests\"")
    set(CONFIG_HEADER "${PROJECT_SOURCE_DIR}/tests/include/jsontests.h")
    configure_file("${PROJECT_SOURCE_DIR}/tests/include/jsontests.h.in" ${CONFIG_HEADER} @ONLY)

    enable_testing()
    add_subdirectory(tests)

endif()
