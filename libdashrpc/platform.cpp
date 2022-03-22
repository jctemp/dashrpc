#include <fmt/color.h>
#include <fmt/core.h>
#include <fstream>
#include <grpcpp/grpcpp.h>

#include <platform.hpp>

// TODO: need better include of libdashprotos
#include "../libdashprotos/platform.grpc.pb.h"
#include "../libdashprotos/platform.pb.h"

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

int32_t platform::broadcast_state_transition(std::string newState)
{
    grpc::ClientContext ctx;
    Dash::BroadcastStateTransitionRequest stateReq;
    Dash::BroadcastStateTransitionResponse stateRes;
    
    connection->stub->broadcastStateTransition(&ctx, stateReq, &stateRes);

    return 0;
}

int32_t platform::wait_for_state_transition_result(void)
{

    grpc::ClientContext* ctx;
    Dash::WaitForStateTransitionResultRequest waitStateReq;
    Dash::WaitForStateTransitionResultResponse waitStateRes;

    connection->stub->waitForStateTransitionResult(&ctx, waitStateReq, &waitStateRes);

    if(waitStateRes.has_metadata()) {

    }

    if(waitStateRes.has_error()) {

    }

    if(waitStateRes.has_proof()) {

    }

    return 0;
}

int32_t platform::identity(void)
{
    grpc::ClientContext ctx;
    Dash::GetIdentityRequest identityReq;
    Dash::GetIdentityResponse identityRes;

    connection->stub->getIdentity(&ctx, identityReq, &identityRes);

    if(identityRes.has_proof()) {

    }

    if(identityRes.has_metadata()) {

    }

    return 0;
}

int32_t platform::data_contract(void)
{

    grpc::ClientContext* ctx;
    Dash::GetDataContractRequest contractReq;
    Dash::GetDataContractResponse contractRes;

    connection->stub->getDataContract(&ctx, contractReq, &contractRes);

    return 0;
}

int32_t platform::documents(void)
{
    grpc::ClientContext ctx;
    Dash::GetDocumentsRequest documentReq;
    Dash::GetDocumentsResponse documentRes;

    connection->stub->getDocuments(&ctx, documentReq, &documentRes);

    return 0;
}

int32_t platform::identities_by_public_key_hashes(void)
{
    grpc::ClientContext ctx;
    Dash::GetIdentitiesByPublicKeyHashesRequest identitiesReq;
    Dash::GetIdentitiesByPublicKeyHashesResponse identitiesRes;

    connection->stub->getIdentitiesByPublicKeyHashes(&ctx, identitiesReq, &identitiesRes);

    return 0;
}

int32_t platform::identity_ids_by_public_key_hashes(void)
{
    grpc::ClientContext ctx;
    Dash::GetIdentityIdsByPublicKeyHashesRequest identityIdReq;
    Dash::GetIdentityIdsByPublicKeyHashesResponse identityIdRes;

    connection->stub->getIdentityIdsByPublicKeyHashes(&ctx, identityIdReq, &identityIdRes);

    return 0;
}

int32_t platform::consensus_params(void)
{
    grpc::ClientContext ctx;
    Dash::GetConsensusParamsRequest consensusReq;
    Dash::GetConsensusParamsResponse consensusRes;

    connection->stub->getConsensusParams(&ctx, consensusReq, &consensusRes);

    return 0;
}

} // namespace dashrpc
