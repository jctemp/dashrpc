cmake_minimum_required(VERSION 3.18)

set(PROJECT "dash_client")
project("${PROJECT}" VERSION 0.1.0 LANGUAGES CXX)

# Global CMake variables are set here
set(CMAKE_CXX_STANDARD              17)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS   ON)

include(FetchContent)

# Fetch the content of the repository

set(RPC_NAME "rpc")
FetchContent_Declare(
    ${RPC_NAME}
    GIT_REPOSITORY "https://github.com/rpclib/rpclib.git"
    GIT_TAG "master"
)

# set(BOOST_NAME "boost")
# FetchContent_Declare(
#     ${BOOST_NAME}
#     GIT_REPOSITORY "https://github.com/boostorg/boost.git"
#     GIT_TAG "master"
# )

set(JSON_NAME "nlohmann_json")
FetchContent_Declare(
    ${JSON_NAME}
    GIT_REPOSITORY "https://github.com/nlohmann/json.git"
    GIT_TAG "master"
)

FetchContent_MakeAvailable(${RPC_NAME} ${JSON_NAME})

add_subdirectory(client)
