cmake_minimum_required(VERSION 3.18)

set(PROJECT "dash_client")
project("${PROJECT}" VERSION 0.1.0 LANGUAGES CXX)

# Global CMake variables are set here
set(CMAKE_CXX_STANDARD              17)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS   ON)

## Fetch the content of the repository ##
# set variables
include(FetchContent)

set(GITHUB_PREFIX "https://github.com")

set(RPC_LIBRARY_NAME "rpc")
set(RPC_LIBRARY_PATH "rpclib/rpclib")

set(JSON_LIBRARY_NAME "nlohmann_json")
set(JSON_LIBRARY_PATH "nlohmann/json")

# do fetch operation
message(STATUS "Fetching ${RPC_LIBRARY_NAME} library")
FetchContent_Declare(
    ${RPC_LIBRARY_NAME}
    GIT_REPOSITORY "${GITHUB_PREFIX}/${RPC_LIBRARY_PATH}.git"
    GIT_TAG "master"
)

message(STATUS "Fetching ${JSON_LIBRARY_NAME} library")
FetchContent_Declare(
    ${JSON_LIBRARY_NAME}
    GIT_REPOSITORY "${GITHUB_PREFIX}/${JSON_LIBRARY_PATH}.git"
    GIT_TAG "master"
)

message(STATUS "Populating project with variables of the following: ${RPC_NAME} ${JSON_NAME}")
FetchContent_MakeAvailable(${RPC_NAME} ${JSON_NAME})

# Subdirectories
add_subdirectory(client)
