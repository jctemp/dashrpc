find_package(Threads REQUIRED)

add_executable(${PROJECT} main.cpp)
target_link_libraries(${PROJECT} Threads::Threads)

# fetch content libraries
target_link_libraries(${PROJECT} ${RPC_LIBRARY_NAME})
target_link_libraries(${PROJECT} ${JSON_LIBRARY_NAME})
