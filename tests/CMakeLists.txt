# TARGETS
set(UNIT_TEST_MAIN "${PROJECT}_unit_tests")

# SOURCES
set(TEST_SOURCES "main.cpp")

# UNIT TESTS EXECUTABLE
add_executable(${UNIT_TEST_MAIN} ${TEST_SOURCES})
target_link_libraries(${UNIT_TEST_MAIN} PRIVATE
    ${LIB_DASHRPC}
    nlohmann_json
    Catch2::Catch2WithMain)

add_test(NAME "${PROJECT_NAME}_functions" COMMAND ${UNIT_TEST_MAIN})
