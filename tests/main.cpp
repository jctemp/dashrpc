#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"
#include "fmt/color.h"
#include "fmt/core.h"
#include "grpcpp/grpcpp.h"
#include "include/jsontests.h"
#include "nlohmann/json.hpp"
#include <string>

#include "dashrpc.hpp"

TEST_CASE("TEXT TO JSON", "libdashrpc")
{
    SECTION("EXCEPTION HANDLING")
    REQUIRE_THROWS_AS(dashrpc::read_json(""), std::logic_error);

    SECTION("SIMPLE JSON OBJECT")
    {
        auto path{std::string{JSON_FILES_DIR "/test0.json"}};

        nlohmann::json json_text{{}};
        json_text = dashrpc::read_json(path);
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
        std::string version{dashrpc::grpc_version()};
        REQUIRE(version.size() > 0);
    }

    SECTION("GRPC LAYER 1 CALL")
    {
    }

    SECTION("GRPC LAYER 2 CALL")
    {
        // connection possible
        dashrpc::grpc_dash_layer();
    }
}
