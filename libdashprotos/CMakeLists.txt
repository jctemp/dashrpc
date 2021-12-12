# SETUP FOR DEPENDENCIES OF THE DASH GRPC AND CPP FILES
set(DASH_PROTOS_DIR "${PROJECT_SOURCE_DIR}/libdashprotos")

set(PROTOC "${CONAN_BIN_DIRS_PROTOBUF}/protoc")
set(GRPC_PLUGIN "${CONAN_BIN_DIRS_GRPC}/grpc_cpp_plugin")

set(CORE_NAME "core.proto")
set(PLATFORM_NAME "platform.proto")

set(LIB_DEPS
    "_core_grpc_deps"
    "_core_cpp_deps"
    "_platform_grpc_deps"
    "_platform_cpp_deps"
)

# SETUP FOR DEPENDENCIES OF THE DASH GRPC AND CPP FILES
add_custom_target(_core_grpc_deps
    ${PROTOC}
        -I ${DASH_PROTOS_DIR}
        --grpc_out=${DASH_PROTOS_DIR}
        --plugin=protoc-gen-grpc=${GRPC_PLUGIN}
        ${DASH_PROTOS_DIR}/${CORE_NAME}
)

add_custom_target(_core_cpp_deps
    ${PROTOC}
        -I ${DASH_PROTOS_DIR}
        --cpp_out=${DASH_PROTOS_DIR}
        ${DASH_PROTOS_DIR}/${CORE_NAME}
)

add_custom_target(_platform_grpc_deps
    ${PROTOC}
        -I ${DASH_PROTOS_DIR}
        --grpc_out=${DASH_PROTOS_DIR}
        --plugin=protoc-gen-grpc=${GRPC_PLUGIN}
        ${DASH_PROTOS_DIR}/${PLATFORM_NAME}
)

add_custom_target(_platform_cpp_deps
    ${PROTOC}
        -I ${DASH_PROTOS_DIR}
        --cpp_out=${DASH_PROTOS_DIR}
        ${DASH_PROTOS_DIR}/${PLATFORM_NAME}
)

# SOURCE AND HEADER FILES
set(DASH_CORE_SOURCE "${DASH_PROTOS_DIR}/core.grpc.pb.cc" "${DASH_PROTOS_DIR}/core.pb.cc")
set(DASH_CORE_HEADER "${DASH_PROTOS_DIR}/core.grpc.pb.h" "${DASH_PROTOS_DIR}/core.pb.h")
set(DASH_PLATFORM_SOURCE "${DASH_PROTOS_DIR}/platform.grpc.pb.cc" "${DASH_PROTOS_DIR}/platform.pb.cc")
set(DASH_PLATFORM_HEADER "${DASH_PROTOS_DIR}/platform.grpc.pb.h" "${DASH_PROTOS_DIR}/platform.pb.h")

# GRPC TARGET
add_library(${LIB_DASHPROTOS}
    ${DASH_CORE_SOURCE} ${DASH_CORE_HEADER}
    ${DASH_PLATFORM_SOURCE} ${DASH_PLATFORM_HEADER}
)
add_dependencies(${LIB_DASHPROTOS} ${LIB_DEPS})
