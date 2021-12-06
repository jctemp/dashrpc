#define CATCH_CONFIG_MAIN

#include <string>

#include "catch2/catch.hpp"
#include "fmt/color.h"
#include "fmt/core.h"
#include "nlohmann/json.hpp"

#include "dashrpc.hpp"

TEST_CASE("TEXT TO JSON", "libdashrpc")
{
    SECTION("EXCEPTION HANDLING")
        REQUIRE_THROWS_AS(read_json(""), std::logic_error);

    SECTION("SIMPLE JSON OBJECT")
    {
        auto path{std::string{"/mnt/d/GIT_REPOSITORIES/dash_client/tests/json_files/test0.json"}};

        nlohmann::json json_text{{}};
        json_text = read_json(path);
        REQUIRE(json_text["happy"].is_boolean());
        REQUIRE(json_text["happy"].get<bool>());
    }
}

TEST_CASE("GRPC COMMANDS", "libdashrpc")
{
    // SECTION("EXCEPTION HANDLING")
    //     REQUIRE_THROWS_AS(send_command("", ""), std::logic_error);

    SECTION("GRPC VERSION")
    {
        std::string version{grpc_version()};
        REQUIRE(version.size() > 0);
    }
}