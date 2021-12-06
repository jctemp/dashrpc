#include "dashrpc.hpp"

#include "fmt/core.h"
#include "grpc/grpc.h"

#include "grpcpp/channel.h"
#include "grpcpp/client_context.h"
#include "grpcpp/security/authorization_policy_provider.h"
#include "grpcpp/security/credentials.h"

bool test_function(void)
{
    fmt::print("THIS IS A TEST FUNCTION");
    return true;
}


void grpc_test(void)
{
    grpc::ChannelArguments test{};
    grpc::CreateCustomChannel("localhost:20002", grpc::InsecureChannelCredentials(), test);
}
