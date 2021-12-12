# SETUP FOR DEPENDENCIES OF THE DASH GRPC AND CPP FILES
set(DASH_PROTOS_DIR "${PROJECT_SOURCE_DIR}/src/protos")

set(LIB_DEPS _core_grpc_deps _core_cpp_deps _platform_grpc_deps _platform_cpp_deps)

add_custom_target(_core_grpc_deps
    ${CONAN_BIN_DIRS_PROTOBUF}/protoc
        -I ${DASH_PROTOS_DIR}
        --grpc_out=${DASH_PROTOS_DIR}
        --plugin=protoc-gen-grpc=${CONAN_BIN_DIRS_GRPC}/grpc_cpp_plugin
        ${DASH_PROTOS_DIR}/core.proto
)

add_custom_target(_core_cpp_deps
    ${CONAN_BIN_DIRS_PROTOBUF}/protoc
        -I ${DASH_PROTOS_DIR}
        --cpp_out=${DASH_PROTOS_DIR}
        ${DASH_PROTOS_DIR}/core.proto
)

add_custom_target(_platform_grpc_deps
    ${CONAN_BIN_DIRS_PROTOBUF}/protoc
        -I ${DASH_PROTOS_DIR}
        --grpc_out=${DASH_PROTOS_DIR}
        --plugin=protoc-gen-grpc=${CONAN_BIN_DIRS_GRPC}/grpc_cpp_plugin
        ${DASH_PROTOS_DIR}/platform.proto
)

add_custom_target(_platform_cpp_deps
    ${CONAN_BIN_DIRS_PROTOBUF}/protoc
        -I ${DASH_PROTOS_DIR}
        --cpp_out=${DASH_PROTOS_DIR}
        ${DASH_PROTOS_DIR}/platform.proto
)

set(DASH_CORE_SOURCE "${DASH_PROTOS_DIR}/core.grpc.pb.cc" "${DASH_PROTOS_DIR}/core.pb.cc")
set(DASH_CORE_HEADER "${DASH_PROTOS_DIR}/core.grpc.pb.h" "${DASH_PROTOS_DIR}/core.pb.h")
set(DASH_PLATFORM_SOURCE "${DASH_PROTOS_DIR}/platform.grpc.pb.cc" "${DASH_PROTOS_DIR}/platform.pb.cc")
set(DASH_PLATFORM_HEADER "${DASH_PROTOS_DIR}/platform.grpc.pb.h" "${DASH_PROTOS_DIR}/platform.pb.h")

add_library(${LIB_DASHGRPC}
    ${DASH_CORE_SOURCE} ${DASH_CORE_HEADER}
    ${DASH_PLATFORM_SOURCE} ${DASH_PLATFORM_HEADER}
)
add_dependencies(${LIB_DASHGRPC} ${LIB_DEPS})

# DASHRPC SOURCE AND HEADERS
set(DASHRPC_SOURCES  "dashrpc.cpp")
set(DASHRPC_HEADER   "${PROJECT_SOURCE_DIR}/include/dashrpc.hpp")
set(DASHRPC_INCLUDES "${PROJECT_SOURCE_DIR}/include/")

# DEFINE LIBRPC AS A TARGET
add_library(${LIB_DASHRPC}
    ${DASHRPC_SOURCES} ${DASHRPC_HEADER}
)

target_include_directories(${LIB_DASHRPC} PUBLIC ${DASHRPC_INCLUDES})
target_link_libraries(${LIB_DASHRPC} PRIVATE ${LIB_DASHGRPC})
target_link_libraries(${LIB_DASHRPC} PRIVATE ${CONAN_LIBS_FMT})
target_link_libraries(${LIB_DASHRPC} PRIVATE ${CONAN_LIBS_GRPC})
target_link_libraries(${LIB_DASHRPC} PRIVATE ${CONAN_LIBS_NLOHMANN_JSON})
