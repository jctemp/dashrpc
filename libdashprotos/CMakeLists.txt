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
set(CORE_GRPC_DEPS_BYPRODUCT "${DASH_PROTOS_DIR}/core.grpc.pb.cc" "${DASH_PROTOS_DIR}/core.grpc.pb.h")
add_custom_target(_core_grpc_deps
    COMMAND ${PROTOC}
        -I ${DASH_PROTOS_DIR}
        --grpc_out=${DASH_PROTOS_DIR}
        --plugin=protoc-gen-grpc=${GRPC_PLUGIN}
        ${DASH_PROTOS_DIR}/${CORE_NAME}
    BYPRODUCTS ${CORE_GRPC_DEPS_BYPRODUCT}
)

set(CORE_DEPS_BYPRODUCT "${DASH_PROTOS_DIR}/core.pb.cc" "${DASH_PROTOS_DIR}/core.pb.h")
add_custom_target(_core_cpp_deps
    ${PROTOC}
        -I ${DASH_PROTOS_DIR}
        --cpp_out=${DASH_PROTOS_DIR}
        ${DASH_PROTOS_DIR}/${CORE_NAME}
    BYPRODUCTS ${CORE_DEPS_BYPRODUCT}
)

set(PLATFORM_GRPC_DEPS_BYPRODUCT "${DASH_PROTOS_DIR}/platform.grpc.pb.cc" "${DASH_PROTOS_DIR}/platform.grpc.pb.h" )
add_custom_target(_platform_grpc_deps
    ${PROTOC}
        -I ${DASH_PROTOS_DIR}
        --grpc_out=${DASH_PROTOS_DIR}
        --plugin=protoc-gen-grpc=${GRPC_PLUGIN}
        ${DASH_PROTOS_DIR}/${PLATFORM_NAME}
    BYPRODUCTS ${PLATFORM_GRPC_DEPS_BYPRODUCT}
)

set(PLATFORM_DEPS_BYPRODUCT "${DASH_PROTOS_DIR}/platform.pb.cc" "${DASH_PROTOS_DIR}/platform.pb.h")
add_custom_target(_platform_cpp_deps
    ${PROTOC}
        -I ${DASH_PROTOS_DIR}
        --cpp_out=${DASH_PROTOS_DIR}
        ${DASH_PROTOS_DIR}/${PLATFORM_NAME}
    BYPRODUCTS ${PLATFORM_DEPS_BYPRODUCT}
)

# SOURCE AND HEADER FILES

# GRPC TARGET
add_library(${LIB_DASHPROTOS}
    ${CORE_GRPC_DEPS_BYPRODUCT}
    ${PLATFORM_GRPC_DEPS_BYPRODUCT}
    ${CORE_DEPS_BYPRODUCT}
    ${PLATFORM_DEPS_BYPRODUCT}
)
add_dependencies(${LIB_DASHPROTOS} ${LIB_DEPS})
