# SETUP FOR DEPENDENCIES OF THE DASH GRPC AND CPP FILES
set(CORE_NAME "core.proto")
set(PLATFORM_NAME "platform.proto")

# Set path to executables (protoc and grpc)
if(ENABLE_DASHRPC_CONAN)
    set(GRPC_PLUGIN "${CONAN_BIN_DIRS_GRPC}/grpc_cpp_plugin")
    set(PROTOC      "${CONAN_BIN_DIRS_PROTOBUF}/protoc")
else()
    set(GRPC_PLUGIN "${PROJECT_BINARY_DIR}/extern/grpc/grpc_cpp_plugin")
    set(PROTOC      "${PROJECT_BINARY_DIR}/extern/grpc/third_party/protobuf/protoc")
endif()

# protofiles

set(PATH_TO_CORE     "../extern/platform/packages/dapi-grpc/protos/core/v0")
set(PATH_TO_PLATFORM "../extern/platform/packages/dapi-grpc/protos/platform/v0")

add_custom_target(core_proto ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink  "${CMAKE_CURRENT_SOURCE_DIR}/${PATH_TO_CORE}/${CORE_NAME}" "${CMAKE_CURRENT_BINARY_DIR}/${CORE_NAME}")

add_custom_target(platform_proto ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink  "${CMAKE_CURRENT_SOURCE_DIR}/${PATH_TO_PLATFORM}/${PLATFORM_NAME}" "${CMAKE_CURRENT_BINARY_DIR}/${PLATFORM_NAME}")

set(proto_path "${CMAKE_CURRENT_BINARY_DIR}")

# create destination folder
set(source_dir_path "${CMAKE_CURRENT_BINARY_DIR}/proto")
if(NOT EXISTS "${source_dir_path}")
    file(MAKE_DIRECTORY "${source_dir_path}")
endif()

# Generated Sources
set(core_proto_source     "${CMAKE_CURRENT_BINARY_DIR}/proto/core.pb.cc")
set(core_proto_header     "${CMAKE_CURRENT_BINARY_DIR}/proto/core.pb.h")
set(platform_proto_source "${CMAKE_CURRENT_BINARY_DIR}/proto/platform.pb.cc")
set(platform_proto_header "${CMAKE_CURRENT_BINARY_DIR}/proto/platform.pb.h")
set(core_grpc_source      "${CMAKE_CURRENT_BINARY_DIR}/proto/core.grpc.pb.cc")
set(core_grpc_header      "${CMAKE_CURRENT_BINARY_DIR}/proto/core.grpc.pb.h")
set(platform_grpc_source  "${CMAKE_CURRENT_BINARY_DIR}/proto/platform.grpc.pb.cc")
set(platform_grpc_header  "${CMAKE_CURRENT_BINARY_DIR}/proto/platform.grpc.pb.h")

# SETUP FOR DEPENDENCIES OF THE DASH GRPC AND CPP FILES
add_custom_command(
    OUTPUT
        "${core_proto_source}"
        "${core_proto_header}"
        "${core_grpc_source}"
        "${core_grpc_header}"
        "${platform_proto_source}"
        "${platform_proto_header}"
        "${platform_grpc_source}"
        "${platform_grpc_header}"
    COMMAND ${PROTOC}
    ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}/proto"
         --cpp_out  "${CMAKE_CURRENT_BINARY_DIR}/proto"
         -I         "${proto_path}"
         --plugin=protoc-gen-grpc=${GRPC_PLUGIN}
         "${CMAKE_CURRENT_BINARY_DIR}/${CORE_NAME}" "${CMAKE_CURRENT_BINARY_DIR}/${PLATFORM_NAME}"
    DEPENDS
         core_proto platform_proto)

find_package(Threads REQUIRED)

# GRPC TARGET
add_library(${LIB_DASHPROTOS}
    "${core_proto_source}"
    "${core_proto_header}"
    "${core_grpc_source}"
    "${core_grpc_header}"
    "${platform_proto_source}"
    "${platform_proto_header}"
    "${platform_grpc_source}"
    "${platform_grpc_header}")

target_include_directories(${LIB_DASHPROTOS} PUBLIC
    "${DASH_PROTOS_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}")

target_link_libraries(${LIB_DASHPROTOS} PRIVATE
    ${LIBS_GRPC}
    ${LIBS_PROTOBUF}
    Threads::Threads)

if(NOT ENABLE_DASHRPC_CONAN)
    add_dependencies(${LIB_DASHPROTOS} ${LIBS_DEPS_PROTOBUF})
endif()
