#include <string>
#include <catch2/catch_all.hpp>
#include <dashrpc/dashrpc.hpp>
#include <future>

TEST_CASE("dashrpc::platform::init", "[libdashrpc]")
{
    bool success = dashrpc::platform::init(); //is connecting
    dashrpc::platform::broadcast_state_transition();
    dashrpc::platform::identity();
    //std::async(dashrpc::platform::data_contract); //error
    dashrpc::platform::documents();
    dashrpc::platform::identities_by_public_key_hashes();
    dashrpc::platform::identity_ids_by_public_key_hashes();
    dashrpc::platform::consensus_params();
    std::async(dashrpc::platform::wait_for_state_transition_result); //only async call

    REQUIRE(success);
}