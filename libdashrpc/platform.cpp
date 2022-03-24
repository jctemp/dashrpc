#include <dashrpc/platform.hpp>

#include <fmt/color.h>
#include <fmt/core.h>
#include <grpcpp/grpcpp.h>

#include <proto/platform.grpc.pb.h>
#include <proto/platform.pb.h>

namespace Dash = org::dash::platform::dapi::v0;

namespace dashrpc
{

struct platform::connection
{
    std::shared_ptr<grpc::Channel> channel;
    std::unique_ptr<Dash::Platform::Stub> stub;
};

void platform::init(std::string address, uint16_t port)
{
    connection->channel = grpc::CreateChannel(fmt::format("{}:{}", address, port), grpc::InsecureChannelCredentials());
    connection->stub = Dash::Platform::NewStub(connection->channel);
}

int32_t platform::broadcast_state_transition(void)
{
    return 0;
}

int32_t platform::identity(void)
{
    return 0;
}

int32_t platform::data_contract(void)
{
    return 0;
}

int32_t platform::documents(void)
{
    return 0;
}

int32_t platform::identities_by_public_key_hashes(void)
{
    return 0;
}

int32_t platform::identity_ids_by_public_key_hashes(void)
{
    return 0;
}

int32_t platform::wait_for_state_transition_result(void)
{
    return 0;
}

int32_t platform::consensus_params(void)
{
    return 0;
}

} // namespace dashrpc
