cmake_minimum_required(VERSION 3.18)

set(PROJECT "dashrpc")
project("${PROJECT}" VERSION 0.2.1 LANGUAGES CXX)

# GLOBAL CMAKE SETTINGS
set(CMAKE_CXX_STANDARD              17)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            OFF)


# OPTIONS
option(ENABLE_DASHRPC_TESTING    "Enable a Unit Testing build." OFF)
option(ENABLE_DASHRPC_CONAN "Use conan for libraries rather then submodules" OFF)

# SUBMODS OR CONAN
if(ENABLE_DASHRPC_CONAN)
    include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    conan_basic_setup()

    # SET TARGET TO LINK AGAINST
    set(LIBS_FMT           ${CONAN_LIBS_FMT})
    set(LIBS_GRPC          ${CONAN_LIBS_GRPC})
    set(LIBS_NLOHMANN_JSON ${CONAN_LIBS_NLOHMANN_JSON})
    set(LIBS_OPENSSL       ${CONAN_LIBS_OPENSSL})
    set(LIBS_PROTOBUF      ${CONAN_LIBS_PROTOBUF})
    set(LIBS_C_ARES        ${CONAN_LIBS_C-ARES})
    set(LIBS_ABSEIL        ${CONAN_LIBS_ABSEIL})
    set(LIBS_RE2           ${CONAN_LIBS_RE2})
    set(LIBS_ZLIB          ${CONAN_LIBS_ZLIB})
    
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
    set(LIBS_GRPC          m pthread grpc++_alts grpc++_error_details grpc_plugin_support
                           grpc++_unsecure grpc_unsecure grpc++_reflection 
                           grpcpp_channelz grpc++ grpc address_sorting gpr upb)
    set(LIBS_NLOHMANN_JSON nlohmann_json::nlohmann_json)
    set(LIBS_OPENSSL       dl pthread rt ssl crypto)
    set(LIBS_PROTOBUF      pthread protoc protobuf)
    set(LIBS_C_ARES        rt cares)
    set(LIBS_ABSEIL        pthread m rt absl_scoped_set_env absl_strerror 
                           absl_failure_signal_handler absl_examine_stack
                           absl_leak_check_disable absl_leak_check absl_flags_parse
                           absl_flags_usage absl_flags_usage_internal absl_flags
                           absl_flags_reflection absl_raw_hash_set absl_hashtablez_sampler
                           absl_flags_private_handle_accessor absl_flags_internal
                           absl_flags_config absl_flags_program_name
                           absl_flags_marshalling absl_flags_commandlineflag
                           absl_flags_commandlineflag_internal absl_hash absl_city
                           absl_low_level_hash absl_periodic_sampler
                           absl_random_distributions absl_random_seed_sequences
                           absl_random_internal_pool_urbg absl_random_seed_gen_exception
                           absl_random_internal_seed_materialabsl_random_internal_randen
                           absl_random_internal_randen_slow absl_random_internal_randen_hwaes
                           absl_random_internal_randen_hwaes_impl
                           absl_random_internal_platform
                           absl_random_internal_distribution_test_util absl_statusor
                           absl_status absl_str_format_internal absl_cordz_sample_token
                           absl_cord absl_cordz_info absl_cord_internal
                           absl_cordz_functions absl_exponential_biased absl_cordz_handle
                           absl_synchronization absl_stacktrace absl_symbolize
                           absl_debugging_internal absl_demangle_internal
                           absl_graphcycles_internal absl_malloc_internal
                           absl_time absl_strings absl_int128 absl_strings_internal
                           absl_base absl_spinlock_wait absl_civil_time absl_time_zone
                           absl_bad_any_cast_impl absl_throw_delegate
                           absl_bad_optional_access absl_bad_variant_access
                           absl_raw_logging_internal absl_log_severity)
    set(LIBS_RE2           m pthread re2)
    set(LIBS_ZLIB          z)
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
