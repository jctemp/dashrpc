#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"

#include "dashrpc.hpp"

TEST_CASE("Test function", "libdashrpc")
{
    REQUIRE(test_function());
}
