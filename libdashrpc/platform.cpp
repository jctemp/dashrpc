#include <iostream>
#include <fstream>

#include <dashrpc/platform.hpp>
#include <nlohmann/json.hpp>

#include <chrono>
#include <ctime>

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

std::unique_ptr<platform::connection> platform::conn{new platform::connection{}};

void debugPrint(std::string msg) {
    std::ofstream outputfile;
    outputfile.open("outputfile.txt", std::ios_base::app);
    outputfile << msg;
    outputfile.close();
}

bool platform::init(std::string address, uint16_t port) //is connecting
{
    debugPrint("--------START----------\n\n");

    debugPrint("--- start init ---\n");
    conn->channel = grpc::CreateChannel(fmt::format("{}:{}", address, port), grpc::InsecureChannelCredentials());
    conn->stub = Dash::Platform::NewStub(conn->channel);

    if (conn->channel == nullptr)
    {
        fmt::print(fmt::fg(fmt::color::red), "Failed to create channel\n");
        debugPrint("Failed to create channel\n");
        return false;
    }
    

    if (conn->stub == nullptr)
    {
        fmt::print(fmt::fg(fmt::color::red), "Failed to create stub\n");
        debugPrint("Failed to create stub\n");
        return false;
    }

    fmt::print(fmt::fg(fmt::color::green), "Connected to {}:{}\n", address, port);
    debugPrint("Connected to " + address + ", " + std::to_string(port) + "\n");

    debugPrint("--- end init ---\n\n");
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
    debugPrint("Metadata:\n");
    debugPrint("\tLast comitted platform state height: " + std::to_string(metaDataRes.height()) + "\n");
    debugPrint("\tMost recent ChainLock on the core chain: " + std::to_string(metaDataRes.core_chain_locked_height()) + "");
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
    debugPrint("--- start transition ---\n");

    std::time_t start, end;
    debugPrint("start and end time objs created\n");

    grpc::ClientContext ctx;
    debugPrint("GRPC client context created\n");
    Dash::BroadcastStateTransitionRequest stateReq;
    Dash::BroadcastStateTransitionResponse stateRes;
    debugPrint("Dash state transition request and response objects created\n");

    stateReq.set_state_transition("pWR0eXBlAmlzaWduYXR1cmV4WEg3TWhFWDQ0Z3JzMVIwTE9XTU5IZjAxWFNpYVFQcUlVZ1JLRXQyMkxHVERsUlUrZ1BwQUlUZk5JUmhXd3IvYTVHd0lzWm1idGdYVVFxcVhjbW9lQWtUOD1qcHVibGljS2V5c4GkYmlkAGRkYXRheCxBdzh2UmYxeFFCTlVLbzNiY2llaHlaR2NhM0hBSThkY0ZvVWJTK3hLb0lITmR0eXBlAGlpc0VuYWJsZWT1bmxvY2tlZE91dFBvaW50eDBLT1VUSHB5YnFPek9DNnhEVUhFWm9uc1lNSVpqcGppTHFZNnkxYmlWNWxRQUFBQUFvcHJvdG9jb2xWZXJzaW9uAA==");
    debugPrint("Set default values for the start transition\n");

    start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    debugPrint("Set start time\n");

    conn->stub->broadcastStateTransition(&ctx, stateReq, &stateRes);
    debugPrint("Sended Transition\n");

    end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    debugPrint("Set start time\n");

    debugPrint("--- end transition ---\n\n");

    return 0;
}

int32_t platform::wait_for_state_transition_result(void)
{
    debugPrint("--- start wait_transition ---\n");
    nlohmann::json obj{};
    std::time_t start, end;
    debugPrint("json obj created\nstart and end time objs created\n");

    grpc::ClientContext *ctx;
    debugPrint("GRPC client context created\n");
    Dash::WaitForStateTransitionResultRequest waitStateReq;
    Dash::WaitForStateTransitionResultResponse waitStateRes;
    debugPrint("Dash wait_transition request and response objects created\n");
    
    waitStateReq.set_state_transition_hash("pWR0eXBlAmlzaWduYXR1cmV4WEg3TWhFWDQ0Z3JzMVIwTE9XTU5IZjAxWFNpYVFQcUlVZ1JLRXQyMkxHVERsUlUrZ1BwQUlUZk5JUmhXd3IvYTVHd0lzWm1idGdYVVFxcVhjbW9lQWtUOD1qcHVibGljS2V5c4GkYmlkAGRkYXRheCxBdzh2UmYxeFFCTlVLbzNiY2llaHlaR2NhM0hBSThkY0ZvVWJTK3hLb0lITmR0eXBlAGlpc0VuYWJsZWT1bmxvY2tlZE91dFBvaW50eDBLT1VUSHB5YnFPek9DNnhEVUhFWm9uc1lNSVpqcGppTHFZNnkxYmlWNWxRQUFBQUFvcHJvdG9jb2xWZXJzaW9uAA==");
    waitStateReq.set_prove(false);
    debugPrint("Set default values for the wait_transition\n");

    start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    debugPrint("Set start time\n");

    conn->stub->waitForStateTransitionResult(ctx, waitStateReq, &waitStateRes);
    debugPrint("Sended wait_transition\n");

    end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    debugPrint("Set end time\n");

    if (waitStateRes.has_metadata())
    {
        PrintMetadataMsg(waitStateRes.metadata());
    }
    debugPrint("Metadata: \n\tHeight:" + std::to_string(waitStateRes.metadata().height()) + "\n");

    if (waitStateRes.has_error())
    {
        auto errorTransition{waitStateRes.error()};
        fmt::print(fmt::fg(fmt::color::red), "\nTRANSITION RESULT ERROR");
        fmt::print("Code={}\n", errorTransition.code());
        fmt::print("Message={}\n", errorTransition.message());
        fmt::print("Data={}\n", errorTransition.data());
    }
    debugPrint("Error transition:\n\tCode:" + std::to_string(waitStateRes.error().code()) + "\n\tMessage:" + waitStateRes.error().message() + "\n");
    debugPrint("\n");

    obj["proof"] = {{"rootTreeProof", waitStateRes.proof().signature_llmq_hash()},
                    {"storeTreeProof",
                     waitStateRes.proof().signature()}}; //TODO: Not sure if these are the right elements of the proof
    obj["metadata"] = {{"height", waitStateRes.metadata().height()},
                       {"coreChainLockedHeight", waitStateRes.metadata().core_chain_locked_height()}};
    debugPrint("Created json\n");

    debugPrint("--- end wait_transition ---\n\n");
    return 0;
}

int32_t platform::identity(void) // test data working
{
    debugPrint("--- start get_identity ---\n");

    nlohmann::json obj{};
    std::time_t start, end;
    debugPrint("json obj created\nstart and end time objs created\n");

    grpc::ClientContext ctx;
    debugPrint("GRPC client context created\n");
    Dash::GetIdentityRequest identityReq;
    Dash::GetIdentityResponse identityRes;
    debugPrint("Dash get_identity request and response objects created\n");

    identityReq.set_id("/W2nISbC47UB7vVZNicsUAF8DjPPl40jSO74HyUdCB0=");
    debugPrint("Set default values for the get_identity\n");

    start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    debugPrint("Set start time\n");

    conn->stub->getIdentity(&ctx, identityReq, &identityRes);
    debugPrint("Sended identity request\n");

    end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    debugPrint("Set end time\n");

    if (!(identityRes.identity().empty()))
    {
        fmt::print(fmt::fg(fmt::color::green), "\nIDENTITY: {}\n", identityRes.identity());
        debugPrint("Identity: " + identityRes.identity() + "\n");
    }

    if (identityRes.has_metadata())
    {
        PrintMetadataMsg(identityRes.metadata());
    }

    obj["identity"] = identityRes.identity();
    obj["metadata"] = {{"height", identityRes.metadata().height()},
                       {"coreChainLockedHeight", identityRes.metadata().core_chain_locked_height()}};
    debugPrint("## JSON ##\n");
    debugPrint("\t" + obj.dump() + "\n");
    debugPrint("## JSON ##\n");

    debugPrint("--- end get_identity ---\n\n");

    return 0;
}

int32_t platform::data_contract(void)
{
    debugPrint("--- start get_data_contract ---\n");

    nlohmann::json obj{};
    std::time_t start, end;
    debugPrint("json obj created\nstart and end time objs created\n");

    grpc::ClientContext *ctx;
    debugPrint("GRPC client context created\n");
    Dash::GetDataContractRequest contractReq;
    Dash::GetDataContractResponse contractRes;
    debugPrint("Dash get_data_contract request and response objects created\n");

    contractReq.set_id("uNsY7l8uGhINKREVl5zk6/I22cHD0vD/HTdfhOGXhEk=");
    contractReq.set_prove(true);
    debugPrint("Set default values for the get_data_contract\n");

    start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    debugPrint("Set start time\n");

    conn->stub->getDataContract(ctx, contractReq, &contractRes);
    debugPrint("Sended contract request\n");

    end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    debugPrint("Set end time\n");

    if (!(contractRes.data_contract().empty()))
    {
        fmt::print(fmt::fg(fmt::color::green), "\nCONTRACT: {}\n", contractRes.data_contract());
        debugPrint("Data contract: " + contractRes.data_contract() + "\n");
    }

    if (contractRes.has_metadata())
    {
        PrintMetadataMsg(contractRes.metadata());
    }

    obj["dataContract"] = contractRes.data_contract();
    obj["metadata"] = {{"height", contractRes.metadata().height()},
                       {"coreChainLockedHeight", contractRes.metadata().core_chain_locked_height()}};
    debugPrint("## JSON ##\n");
    debugPrint("\t" + obj.dump() + "\n");
    debugPrint("## JSON ##\n");

    debugPrint("--- start get_data_contract ---\n\n");

    return 0;
}

int32_t platform::documents(void) //test data working
{
    debugPrint("--- start get_documents ---\n");

    nlohmann::json obj{};
    std::time_t start, end;
    debugPrint("json obj created\nstart and end time objs created\n");

    grpc::ClientContext ctx;
    debugPrint("GRPC client context created\n");
    Dash::GetDocumentsRequest documentReq;
    Dash::GetDocumentsResponse documentRes;
    debugPrint("Dash get_documents request and response objects created\n");

    documentReq.set_data_contract_id("uNsY7l8uGhINKREVl5zk6/I22cHD0vD/HTdfhOGXhEk=");
    documentReq.set_document_type("note");
    documentReq.set_limit(10);
    documentReq.set_prove(true);
    debugPrint("Set default values for the get_documents\n");

    start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    debugPrint("Set start time\n");

    conn->stub->getDocuments(&ctx, documentReq, &documentRes);
    debugPrint("Sended documents request\n");

    end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    debugPrint("Set end time\n");

    if (documentRes.documents_size() > 0)
    {
        fmt::print(fmt::fg(fmt::color::green), "\nDOCUMENT SIZE: {} byte\n", documentRes.documents_size());
        debugPrint("documents size: " + std::to_string(documentRes.documents_size()) + "\n");
    }

    if (documentRes.has_metadata())
    {
        PrintMetadataMsg(documentRes.metadata());
    }

    obj["dataContract"] = {documentRes.documents()};
    obj["metadata"] = {{"height", documentRes.metadata().height()},
                       {"coreChainLockedHeight", documentRes.metadata().core_chain_locked_height()}};
    debugPrint("## JSON ##\n");
    debugPrint("\t" + obj.dump() + "\n");
    debugPrint("## JSON ##\n");

    debugPrint("--- end get_documents ---\n\n");

    return 0;
}

int32_t platform::identities_by_public_key_hashes(void) //test data working
{
    debugPrint("--- start get_identities_by_key_hashes ---\n");

    nlohmann::json obj{};
    std::time_t start, end;
    debugPrint("json obj created\nstart and end time objs created\n");

    grpc::ClientContext ctx;
    debugPrint("GRPC client context created\n");
    Dash::GetIdentitiesByPublicKeyHashesRequest identitiesReq;
    Dash::GetIdentitiesByPublicKeyHashesResponse identitiesRes;
    debugPrint("Dash get_identities_by_key_hashes request and response objects created\n");

    identitiesReq.add_public_key_hashes("tRsgcpcDd6IKC/OTHmagRqWHoio=");
    identitiesReq.set_prove(true);
    debugPrint("Set default values for the get_identities_by_key_hashes\n");

    start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    debugPrint("Set start time\n");

    conn->stub->getIdentitiesByPublicKeyHashes(&ctx, identitiesReq, &identitiesRes);
    debugPrint("Sended identity request\n");

    end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    debugPrint("Set end time\n");

    if (identitiesRes.identities().size() > 0)
    {
        debugPrint("Identities:\n");
        std::string sIdentities = "";
        for (auto identity : identitiesRes.identities())
        {
            debugPrint("\t - " + identity + "\n");
            sIdentities += identity;
        }

        fmt::print(fmt::fg(fmt::color::green), "\nIDENTITIES: {}\n", sIdentities);
    }

    if (identitiesRes.identities_size() > 0)
    {
        fmt::print(fmt::fg(fmt::color::green), "\nIDENTITY SIZE: {} byte\n", identitiesRes.identities_size());
        debugPrint("Identities_size: " + std::to_string(identitiesRes.identities_size()) + "\n");
    }

    if (identitiesRes.has_metadata())
    {
        PrintMetadataMsg(identitiesRes.metadata());
    }

    obj["identities"] = {identitiesRes.identities()};
    obj["metadata"] = {{"height", identitiesRes.metadata().height()},
                       {"coreChainLockedHeight", identitiesRes.metadata().core_chain_locked_height()}};
    debugPrint("## JSON ##\n");
    debugPrint("\t" + obj.dump() + "\n");
    debugPrint("## JSON ##\n");

    debugPrint("--- end get_identities_by_key_hashes ---\n\n");

    return 0;
}

int32_t platform::identity_ids_by_public_key_hashes(void) //test data working
{
    debugPrint("--- start get_ids_by_key_hashes ---\n");

    nlohmann::json obj{};
    std::time_t start, end;
    debugPrint("json obj created\nstart and end time objs created\n");

    grpc::ClientContext ctx;
    debugPrint("GRPC client context created\n");
    Dash::GetIdentityIdsByPublicKeyHashesRequest identityIdReq;
    Dash::GetIdentityIdsByPublicKeyHashesResponse identityIdRes;
    debugPrint("Dash get_ids_by_key_hashes request and response objects created\n");

    identityIdReq.add_public_key_hashes("tRsgcpcDd6IKC/OTHmagRqWHoio=");
    identityIdReq.set_prove(true);
    debugPrint("Set default values for the get_ids_by_key_hashes\n");

    start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    debugPrint("Set start time\n");

    conn->stub->getIdentityIdsByPublicKeyHashes(&ctx, identityIdReq, &identityIdRes);
    debugPrint("Sended identity request\n");

    end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    debugPrint("Set end time\n");

    if (!(identityIdRes.identity_ids().empty()))
    {
        debugPrint("IDS:\n");
        std::string sIds = "";
        for (auto identityId : identityIdRes.identity_ids())
        {
            debugPrint("\t - " + identityId + "\n");
            sIds += identityId;
        }
        fmt::print(fmt::fg(fmt::color::green), "\nIDENTITIES: {}\n", sIds);
    }

    if (identityIdRes.identity_ids_size() > 0)
    {
        debugPrint("Identity_size: " + std::to_string(identityIdRes.identity_ids_size()));
        fmt::print(fmt::fg(fmt::color::green), "\nIDENTITY SIZE: {} byte\n", identityIdRes.identity_ids_size());
    }

    if (identityIdRes.has_metadata())
    {
        PrintMetadataMsg(identityIdRes.metadata());
    }

    obj["identityIds"] = {identityIdRes.identity_ids()};
    obj["metadata"] = {{"height", identityIdRes.metadata().height()},
                       {"coreChainLockedHeight", identityIdRes.metadata().core_chain_locked_height()}};
    debugPrint("## JSON ##\n");
    debugPrint("\t" + obj.dump() + "\n");
    debugPrint("## JSON ##\n");

    debugPrint("--- end get_ids_by_key_hashes ---\n\n");

    return 0;
}

int32_t platform::consensus_params(void) //test data working
{
    debugPrint("--- start consensus_param ---\n");

    nlohmann::json obj{};
    std::time_t start, end;
    debugPrint("json obj created\nstart and end time objs created\n");

    grpc::ClientContext ctx;
    debugPrint("GRPC client context created\n");
    Dash::GetConsensusParamsRequest consensusReq;
    Dash::GetConsensusParamsResponse consensusRes;
    debugPrint("Dash consensus_param request and response objects created\n");

    consensusReq.set_prove(true);
    debugPrint("Set default values for the consensus_param\n");

    start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    debugPrint("Set start time\n");

    conn->stub->getConsensusParams(&ctx, consensusReq, &consensusRes);
    debugPrint("Sended consensus request\n");

    end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    debugPrint("Set end time\n");

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

    debugPrint("--- end consensus_param ---\n\n");

    return 0;
}

} // namespace dashrpc
