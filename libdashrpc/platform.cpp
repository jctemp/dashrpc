#include <iostream>
#include <fstream>

#include <dashrpc/platform.hpp>
#include <nlohmann/json.hpp>

#include <chrono>
#include <ctime>

#include <fmt/color.h>
#include <fmt/core.h>
//#include <grpcpp/grpcpp.h>

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

std::unique_ptr<platform::connection> platform::conn{new platform::connection{}};

bool platform::init(std::string address, uint16_t port) //is connecting
{
    conn->channel = grpc::CreateChannel(fmt::format("{}:{}", address, port), grpc::InsecureChannelCredentials());
    conn->stub = Dash::Platform::NewStub(conn->channel);

    if (conn->channel == nullptr)
    {
        fmt::print(fmt::fg(fmt::color::red), "Failed to create channel\n");
        return false;
    }
    

    if (conn->stub == nullptr)
    {
        fmt::print(fmt::fg(fmt::color::red), "Failed to create stub\n");
        return false;
    }

    fmt::print(fmt::fg(fmt::color::green), "Connected to {}:{}\n", address, port);
    return true;
}

std::string formatDate(std::time_t time)
{
    std::string timestamp(30, '\0');
    std::strftime(&timestamp[0], timestamp.size(), "%Y-%m-%d %H:%M:%S", std::localtime(&time));
    return timestamp;
}

void PrintMetadataMsg(Dash::ResponseMetadata metaDataRes)
{
    fmt::print(fmt::fg(fmt::color::green), "\nMETADATA TRANSITION RESULT\n");
    fmt::print("Last comitted platform state height={}\n", metaDataRes.height());
    fmt::print("Most recent ChainLock on the core chain{}\n", metaDataRes.core_chain_locked_height());
}

void PrintProofMsg(Dash::Proof proofRes)
{ //Verify that data is being  valid
    fmt::print(fmt::fg(fmt::color::green), "\nTRANSITION RESULT PROOF\n");
    fmt::print("llmq-hash={}\n", proofRes.signature_llmq_hash());
    fmt::print("Signature={}\n", proofRes.signature());
}

int32_t platform::broadcast_state_transition(void) //test data working
{
    std::time_t start, end;

    grpc::ClientContext ctx;
    Dash::BroadcastStateTransitionRequest stateReq;
    Dash::BroadcastStateTransitionResponse stateRes;

    std::string state_transition_hash = "pWR0eXBlAmlzaWduYXR1cmV4WEg3TWhFWDQ0Z3JzMVIwTE9XTU5IZjAxWFNpYVFQcUlVZ1JLRXQyMkxHVERsUlUrZ1BwQUlUZk5JUmhXd3IvYTVHd0lzWm1idGdYVVFxcVhjbW9lQWtUOD1qcHVibGljS2V5c4GkYmlkAGRkYXRheCxBdzh2UmYxeFFCTlVLbzNiY2llaHlaR2NhM0hBSThkY0ZvVWJTK3hLb0lITmR0eXBlAGlpc0VuYWJsZWT1bmxvY2tlZE91dFBvaW50eDBLT1VUSHB5YnFPek9DNnhEVUhFWm9uc1lNSVpqcGppTHFZNnkxYmlWNWxRQUFBQUFvcHJvdG9jb2xWZXJzaW9uAA==";
    stateReq.set_state_transition(state_transition_hash);

    start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    conn->stub->broadcastStateTransition(&ctx, stateReq, &stateRes);

    end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    return 0;
}

int32_t platform::wait_for_state_transition_result()
{
    nlohmann::json obj{};
    std::time_t start, end;

    grpc::ClientContext ctx;
    Dash::WaitForStateTransitionResultRequest waitStateReq;
    Dash::WaitForStateTransitionResultResponse waitStateRes;
    
    std::string state_transition_hash = "iuk7icJyRV886NAdupmjooyVUCYqYCxrpE3gjlRdOqk=";
    waitStateReq.set_allocated_state_transition_hash(&state_transition_hash);
    waitStateReq.set_state_transition_hash(state_transition_hash);

    start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    conn->stub->waitForStateTransitionResult(&ctx, waitStateReq, &waitStateRes);

    end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    if (waitStateRes.has_metadata())
    {
        PrintMetadataMsg(waitStateRes.metadata());
    }

    if (waitStateRes.has_error())
    {
        auto errorTransition{waitStateRes.error()};
        fmt::print(fmt::fg(fmt::color::red), "\nTRANSITION RESULT ERROR");
        fmt::print("Code={}\n", errorTransition.code());
        fmt::print("Message={}\n", errorTransition.message());
        fmt::print("Data={}\n", errorTransition.data());
    }

    obj["proof"] = {{"rootTreeProof", waitStateRes.proof().signature_llmq_hash()},
                    {"storeTreeProof",
                     waitStateRes.proof().signature()}}; //TODO: Not sure if these are the right elements of the proof
    obj["metadata"] = {{"height", waitStateRes.metadata().height()},
                       {"coreChainLockedHeight", waitStateRes.metadata().core_chain_locked_height()}};

    return 0;
}

int32_t platform::identity(void) // test data working
{
    nlohmann::json obj{};
    std::time_t start, end;

    grpc::ClientContext ctx;
    Dash::GetIdentityRequest identityReq;
    Dash::GetIdentityResponse identityRes;

    identityReq.set_id("/W2nISbC47UB7vVZNicsUAF8DjPPl40jSO74HyUdCB0=");

    start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    conn->stub->getIdentity(&ctx, identityReq, &identityRes);

    end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    if (!(identityRes.identity().empty()))
    {
        fmt::print(fmt::fg(fmt::color::green), "\nIDENTITY: {}\n", identityRes.identity());
    }

    if (identityRes.has_metadata())
    {
        PrintMetadataMsg(identityRes.metadata());
    }

    obj["identity"] = identityRes.identity();
    obj["metadata"] = {{"height", identityRes.metadata().height()},
                       {"coreChainLockedHeight", identityRes.metadata().core_chain_locked_height()}};

    return 0;
}

int32_t platform::data_contract(void)
{
    nlohmann::json obj{};
    std::time_t start, end;

    grpc::ClientContext *ctx;
    Dash::GetDataContractRequest contractReq;
    Dash::GetDataContractResponse contractRes;

    contractReq.set_id("uNsY7l8uGhINKREVl5zk6/I22cHD0vD/HTdfhOGXhEk=");
    contractReq.set_prove(false);

    start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    conn->stub->getDataContract(ctx, contractReq, &contractRes);

    end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    if (!(contractRes.data_contract().empty()))
    {
        fmt::print(fmt::fg(fmt::color::green), "\nCONTRACT: {}\n", contractRes.data_contract());
    }

    if (contractRes.has_metadata())
    {
        PrintMetadataMsg(contractRes.metadata());
    }

    obj["dataContract"] = contractRes.data_contract();
    obj["metadata"] = {{"height", contractRes.metadata().height()},
                       {"coreChainLockedHeight", contractRes.metadata().core_chain_locked_height()}};

    return 0;
}

int32_t platform::documents(void) //test data working
{
    nlohmann::json obj{};
    std::time_t start, end;

    grpc::ClientContext ctx;
    Dash::GetDocumentsRequest documentReq;
    Dash::GetDocumentsResponse documentRes;

    documentReq.set_data_contract_id("uNsY7l8uGhINKREVl5zk6/I22cHD0vD/HTdfhOGXhEk=");
    documentReq.set_document_type("note");
    documentReq.set_limit(10);
    documentReq.set_prove(true);

    start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    conn->stub->getDocuments(&ctx, documentReq, &documentRes);

    end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    if (documentRes.documents_size() > 0)
    {
        fmt::print(fmt::fg(fmt::color::green), "\nDOCUMENT SIZE: {} byte\n", documentRes.documents_size());
    }

    if (documentRes.has_metadata())
    {
        PrintMetadataMsg(documentRes.metadata());
    }

    obj["dataContract"] = {documentRes.documents()};
    obj["metadata"] = {{"height", documentRes.metadata().height()},
                       {"coreChainLockedHeight", documentRes.metadata().core_chain_locked_height()}};

    return 0;
}

int32_t platform::identities_by_public_key_hashes(void) //test data working
{
    nlohmann::json obj{};
    std::time_t start, end;

    grpc::ClientContext ctx;
    Dash::GetIdentitiesByPublicKeyHashesRequest identitiesReq;
    Dash::GetIdentitiesByPublicKeyHashesResponse identitiesRes;

    identitiesReq.add_public_key_hashes("tRsgcpcDd6IKC/OTHmagRqWHoio=");
    identitiesReq.set_prove(true);

    start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    conn->stub->getIdentitiesByPublicKeyHashes(&ctx, identitiesReq, &identitiesRes);

    end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    if (identitiesRes.identities().size() > 0)
    {
        std::string sIdentities = "";
        for (auto identity : identitiesRes.identities())
        {
            sIdentities += identity;
        }

        fmt::print(fmt::fg(fmt::color::green), "\nIDENTITIES: {}\n", sIdentities);
    }

    if (identitiesRes.identities_size() > 0)
    {
        fmt::print(fmt::fg(fmt::color::green), "\nIDENTITY SIZE: {} byte\n", identitiesRes.identities_size());
    }

    if (identitiesRes.has_metadata())
    {
        PrintMetadataMsg(identitiesRes.metadata());
    }

    obj["identities"] = {identitiesRes.identities()};
    obj["metadata"] = {{"height", identitiesRes.metadata().height()},
                       {"coreChainLockedHeight", identitiesRes.metadata().core_chain_locked_height()}};

    return 0;
}

int32_t platform::identity_ids_by_public_key_hashes(void) //test data working
{
    nlohmann::json obj{};
    std::time_t start, end;

    grpc::ClientContext ctx;
    Dash::GetIdentityIdsByPublicKeyHashesRequest identityIdReq;
    Dash::GetIdentityIdsByPublicKeyHashesResponse identityIdRes;

    identityIdReq.add_public_key_hashes("tRsgcpcDd6IKC/OTHmagRqWHoio=");
    identityIdReq.set_prove(true);

    start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    conn->stub->getIdentityIdsByPublicKeyHashes(&ctx, identityIdReq, &identityIdRes);

    end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    if (!(identityIdRes.identity_ids().empty()))
    {
        std::string sIds = "";
        for (auto identityId : identityIdRes.identity_ids())
        {
            sIds += identityId;
        }
        fmt::print(fmt::fg(fmt::color::green), "\nIDENTITIES: {}\n", sIds);
    }

    if (identityIdRes.identity_ids_size() > 0)
    {
        fmt::print(fmt::fg(fmt::color::green), "\nIDENTITY SIZE: {} byte\n", identityIdRes.identity_ids_size());
    }

    if (identityIdRes.has_metadata())
    {
        PrintMetadataMsg(identityIdRes.metadata());
    }

    obj["identityIds"] = {identityIdRes.identity_ids()};
    obj["metadata"] = {{"height", identityIdRes.metadata().height()},
                       {"coreChainLockedHeight", identityIdRes.metadata().core_chain_locked_height()}};

    return 0;
}

int32_t platform::consensus_params(void) //test data working
{
    nlohmann::json obj{};
    std::time_t start, end;

    grpc::ClientContext ctx;
    Dash::GetConsensusParamsRequest consensusReq;
    Dash::GetConsensusParamsResponse consensusRes;

    consensusReq.set_prove(true);

    start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    conn->stub->getConsensusParams(&ctx, consensusReq, &consensusRes);

    end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    if (consensusRes.has_block())
    {
        auto conBlock{consensusRes.block()};
        fmt::print(fmt::fg(fmt::color::green), "\nCONSENSUS BLOCK\n");
        fmt::print("Max-bytes={}\n", conBlock.max_bytes());
        fmt::print("Max-gas={}\n", conBlock.max_gas());
        fmt::print("Time-iota-ms={}\n", conBlock.time_iota_ms());
    }

    if (consensusRes.has_evidence())
    {
        auto conEvidence{consensusRes.evidence()};
        fmt::print(fmt::fg(fmt::color::green), "\nCONSENSUS EVIDENCE\n");
        fmt::print("Max-age-num-blocks={}\n", conEvidence.max_age_num_blocks());
        fmt::print("Max-age-duration={}\n", conEvidence.max_age_duration());
        fmt::print("Max-bytes={}\n", conEvidence.max_bytes());
    }

    return 0;
}

} // namespace dashrpc
