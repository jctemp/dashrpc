# DASHRPC SOURCE
set(DASHRPC_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/dashrpc.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/platform.cpp")

# DASHRPC HEADERS
set(DASHRPC_HEADER
    "${CMAKE_CURRENT_SOURCE_DIR}/include/dashrpc.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/core.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/platform.hpp")

# DASHRPC INCLUDE DIRS
set(DASHRPC_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include/")

# DEFINE LIBRPC AS A TARGET
add_library(${LIB_DASHRPC} ${DASHRPC_SOURCES} ${DASHRPC_HEADER})

# MAKE DASHPROTOS FILES AS DEPENDENCY
add_dependencies(${LIB_DASHRPC} ${LIB_DASHPROTOS})

# SET INCLUDE DIR FOR OTHER LINKING TARGETS
target_include_directories(${LIB_DASHRPC} PUBLIC ${DASHRPC_INCLUDES})

# LINK AGAINS DEPENDENCIES
target_link_libraries(${LIB_DASHRPC} PRIVATE
    # grpc and proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF}
    # io and json
    fmt::fmt
    nlohmann_json
    # proto deps
    ${LIB_DASHPROTOS})
