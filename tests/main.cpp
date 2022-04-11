#include <string>
#include <catch2/catch_all.hpp>
#include <dashrpc/dashrpc.hpp>


TEST_CASE("dashrpc::platform::init", "[libdashrpc]")
{
    bool success = dashrpc::platform::init();
    REQUIRE(success);
}