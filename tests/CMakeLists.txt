# Sources
set(TEST_SOURCES "main.cpp")

# Executable
add_executable(${TEST_MAIN} ${TEST_SOURCES})

# Settings
target_link_libraries(${TEST_MAIN} ${LIB_DASHRPC})

# Test
add_test(NAME ${LIBRARY_NAME}.${TEST_MAIN} COMMAND ${TEST_MAIN})
