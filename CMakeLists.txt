cmake_minimum_required(VERSION 3.18)

set(PROJECT "dashrpc")
project("${PROJECT}" VERSION 0.2.0 LANGUAGES CXX)

# GLOBAL CMAKE SETTINGS
set(CMAKE_CXX_STANDARD              17)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS   ON)

# OPTIONS
option(ENABLE_TESTING_DASHRPC "Enable a Unit Testing build." OFF)

# SUBMODULES
set(PATH_CATCH "extern/Catch2")
add_subdirectory(${PATH_CATCH})

set(PATH_FMT   "extern/fmt")
add_subdirectory(${PATH_FMT})

set(ABSL_PROPAGATE_CXX_STD ON)
set(PATH_GRPC  "extern/grpc")
add_subdirectory(${PATH_GRPC})
include_directories("${PATH_GRPC}/include")
include_directories("${PATH_GRPC}/third_party/protobuf/src")
include_directories("${PATH_GRPC}/third_party/abseil-cpp")

set(PATH_JSON  "extern/json")
add_subdirectory(${PATH_JSON})
include_directories("${PATH_JSON}/include")

# LIBRARY AND SUBDIRECTORY
set(LIB_DASHPROTOS "dashprotos")
set(LIB_DASHRPC "dashrpc")
add_subdirectory(libdashprotos)
add_subdirectory(libdashrpc)

# UNIT TESTS
include(CTest)
if(ENABLE_TESTING_DASHRPC)
    enable_testing()
    add_subdirectory(tests)
endif()
