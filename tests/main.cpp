#include <string>
#include <catch2/catch_all.hpp>
#include <dashrpc/dashrpc.hpp>


TEST_CASE("dashrpc::platform::init", "[libdashrpc]")
{
    bool success = dashrpc::platform::init(); //is connecting
    dashrpc::platform::broadcast_state_transition();
    //dashrpc::platform::wait_for_state_transition_result(); //error
    dashrpc::platform::identity();
    //dashrpc::platform::data_contract(); //error
    dashrpc::platform::documents();
    dashrpc::platform::identities_by_public_key_hashes();
    dashrpc::platform::identity_ids_by_public_key_hashes();
    dashrpc::platform::consensus_params();

    REQUIRE(success);
}