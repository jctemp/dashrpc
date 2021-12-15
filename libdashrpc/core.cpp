#include <fmt/color.h>
#include <fmt/core.h>
#include <fstream>
#include <grpcpp/grpcpp.h>

#include <core.hpp>

// TODO: need better include of libdashprotos
#include "../libdashprotos/core.grpc.pb.h"
#include "../libdashprotos/core.pb.h"

namespace Dash = org::dash::platform::dapi::v0;

namespace dashrpc
{

struct core::connection
{
    std::shared_ptr<grpc::Channel> channel;
    std::unique_ptr<Dash::Core::Stub> stub;
};

void core::init(std::string address, uint16_t port)
{
    connection->channel = grpc::CreateChannel(fmt::format("{}:{}", address, port), grpc::InsecureChannelCredentials());
    connection->stub = Dash::Core::NewStub(connection->channel);
}

int32_t core::status(void)
{
    return 0;
}

int32_t core::block(void)
{
    return 0;
}

int32_t core::broadcast_transaction(void)
{
    return 0;
}

int32_t core::transaction(void)
{
    return 0;
}

int32_t core::estimated_transaction_fee(void)
{
    return 0;
}

int32_t core::subscribe_to_block_header_with_chain_locks(void)
{
    return 0;
}

int32_t core::subscribe_to_transaction_with_proofs(void)
{
    return 0;
}

} // namespace dashrpc
