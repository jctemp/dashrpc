# sources and headers
set(DASHRPC_SOURCES  "dashrpc.cpp")
set(DASHRPC_HEADER   "${PROJECT_SOURCE_DIR}/include/dashrpc.hpp")
set(DASHRPC_INCLUDES "${PROJECT_SOURCE_DIR}/include/")

# libdashrpc
add_library(${LIB_DASHRPC} ${DASHRPC_SOURCES} ${DASHRPC_HEADER})
target_include_directories(${LIB_DASHRPC} PUBLIC ${DASHRPC_INCLUDES})

target_link_libraries(${LIB_DASHRPC} PUBLIC ${CONAN_LIBS_GRPC})
target_link_libraries(${LIB_DASHRPC} PUBLIC ${CONAN_LIBS_FMT})

target_compile_features(${LIB_DASHRPC} PUBLIC cxx_std_17)
