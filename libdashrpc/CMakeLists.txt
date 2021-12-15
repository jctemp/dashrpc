set(LIBDASHRPC_DIR "${PROJECT_SOURCE_DIR}/libdashrpc")

# DASHRPC SOURCE AND HEADERS
set(DASHRPC_SOURCES
    "${LIBDASHRPC_DIR}/dashrpc.cpp"
    "${LIBDASHRPC_DIR}/core.cpp"
    "${LIBDASHRPC_DIR}/platform.cpp"
)
set(DASHRPC_HEADER
    "${LIBDASHRPC_DIR}/include/dashrpc.hpp"
    "${LIBDASHRPC_DIR}/include/core.hpp"
    "${LIBDASHRPC_DIR}/include/platform.hpp"
)
set(DASHRPC_INCLUDES
    "${LIBDASHRPC_DIR}/include/"
)

# DEFINE LIBRPC AS A TARGET
add_library(${LIB_DASHRPC} ${DASHRPC_SOURCES} ${DASHRPC_HEADER})
add_dependencies(${LIB_DASHRPC} ${LIB_DASHPROTOS})

target_include_directories(${LIB_DASHRPC} PUBLIC ${DASHRPC_INCLUDES})

target_link_libraries(${LIB_DASHPROTOS} PRIVATE ${CONAN_LIBS_ABSEIL})
target_link_libraries(${LIB_DASHPROTOS} PRIVATE ${CONAN_LIBS_OPENSSL})
target_link_libraries(${LIB_DASHPROTOS} PRIVATE ${CONAN_LIBS_C-ARES})
target_link_libraries(${LIB_DASHPROTOS} PRIVATE ${CONAN_LIBS_RE2})
target_link_libraries(${LIB_DASHPROTOS} PRIVATE ${CONAN_LIBS_ZLIB})
target_link_libraries(${LIB_DASHPROTOS} PRIVATE ${CONAN_LIBS_PROTOBUF})
target_link_libraries(${LIB_DASHPROTOS} PRIVATE ${CONAN_LIBS_GRPC})

target_link_libraries(${LIB_DASHRPC} PRIVATE ${LIB_DASHPROTOS})
target_link_libraries(${LIB_DASHRPC} PRIVATE ${CONAN_LIBS_FMT})
target_link_libraries(${LIB_DASHRPC} PRIVATE ${CONAN_LIBS_NLOHMANN_JSON})
