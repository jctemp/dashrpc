cmake_minimum_required(VERSION 3.18)

set(PROJECT "dashrpc")
project("${PROJECT}" VERSION 0.2.0 LANGUAGES CXX)

# GLOBAL CMAKE SETTINGS
set(CMAKE_CXX_STANDARD              17)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS   ON)

# OPTIONS
option(ENABLE_DASHRPC_TESTING    "Enable a Unit Testing build." OFF)
option(ENABLE_DASHRPC_CONAN "Use conan for libraries rather then submodules" OFF)

# SUBMODS OR CONAN
if(ENABLE_DASHRPC_CONAN)
    include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    conan_basic_setup()

    # SET TARGET TO LINK AGAINST
    set(LIBS_FMT      ${CONAN_LIBS_FMT})
    set(LIBS_JSON     ${CONAN_LIBS_NLOHMANN_JSON})
    set(LIBS_GRPC     ${CONAN_LIBS_GRPC})
    set(LIBS_PROTOBUF ${CONAN_LIBS_PROTOBUF})
else()
    # SUBMODULES
    set(PATH_FMT   "extern/fmt")
    set(PATH_GRPC  "extern/grpc")
    set(PATH_JSON  "extern/json")
    set(ABSL_PROPAGATE_CXX_STD ON)

    add_subdirectory(${PATH_FMT})
    add_subdirectory(${PATH_GRPC})
    add_subdirectory(${PATH_JSON})

    include_directories("${PATH_GRPC}/include")
    include_directories("${PATH_GRPC}/third_party/protobuf/src")
    include_directories("${PATH_GRPC}/third_party/abseil-cpp")
    include_directories("${PATH_JSON}/include")

    # SET TARGET TO LINK AGAINST
    set(LIBS_FMT           fmt::fmt)
    set(LIBS_PROTOBUF      protobuf)
    set(LIBS_DEPS_PROTOBUF protoc)
    set(LIBS_JSON          nlohmann_json::nlohmann_json)
    set(LIBS_GRPC          grpc++_alts grpc++_error_details grpc_plugin_support
                           grpc++_unsecur grpc_unsecure grpc++_reflection grpcpp_channelz
                           grpc++ grpc address_sorting gpr upb)
endif()

# REQUIRED
set(PATH_CATCH "extern/Catch2")
add_subdirectory(${PATH_CATCH})
set(LIBS_CATCH         Catch2::Catch2WithMain Catch2::Catch2)

# LIBRARY AND SUBDIRECTORY
set(LIB_DASHPROTOS "dashprotos")
set(LIB_DASHRPC    "dashrpc")
add_subdirectory(libdashprotos)
add_subdirectory(libdashrpc)

# UNIT TESTS
include(CTest)
if(ENABLE_DASHRPC_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
