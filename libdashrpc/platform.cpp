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

    void platform::init(std::string address, uint16_t port)
    {
        connection->channel = grpc::CreateChannel(fmt::format("{}:{}", address, port), grpc::InsecureChannelCredentials());
        connection->stub = Dash::Platform::NewStub(connection->channel);
    }

    std::string formatDate(std::time_t time)
    {
        std::string timestamp (30, '\0');
        std::strftime(&timestamp[0], timestamp.size(), "%Y-%m-%d %H:%M:%S", std::localtime(&time));
        return timestamp;
    }

    void platform::PrintMetadataMsg(auto metaDataRes) {
        fmt::print(fmt::fg(fmt::color::green), "\nMETADATA TRANSITION RESULT\n");
        fmt::print("Last comitted platform state height={}\n", metaDataRes.height());
        fmt::print("Most recent ChainLock on the core chain{}\n", metaDataRes.core_chain_locked_height());
    }

    void platform::PrintProofMsg(auto proofRes) { //Verify that data is being  valid
        fmt::print(fmt::fg(fmt::color::green), "\nTRANSITION RESULT PROOF\n");
        fmt::print("Root-Tree={}\n", proofRes.root_tree_proof());
        fmt::print("llmq-hash={}\n", proofRes.signature_llmq_hash());
        fmt::print("Signature={}\n", proofRes.signature());
        fmt::print("Store-Tree={}\n", proofRes.store_tree_proofs());
    }

    int32_t platform::broadcast_state_transition(void)
    {
        std::time_t start, end;

        grpc::ClientContext ctx;
        Dash::BroadcastStateTransitionRequest stateReq;
        Dash::BroadcastStateTransitionResponse stateRes;
        
        start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        connection->stub->broadcastStateTransition(&ctx, stateReq, &stateRes);

        end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        return 0;
    }

    int32_t platform::wait_for_state_transition_result(void)
    {
        nlohmann::json obj{};
        std::time_t start, end;

        grpc::ClientContext* ctx;
        Dash::WaitForStateTransitionResultRequest waitStateReq;
        Dash::WaitForStateTransitionResultResponse waitStateRes;

        start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        connection->stub->waitForStateTransitionResult(ctx, waitStateReq, &waitStateRes);

        end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        if(waitStateRes.has_metadata()) {
            PrintMetadataMsg(waitStateRes.metadata());
        }

        if(waitStateRes.has_error()) {
            auto errorTransition{waitStateRes.error()};
            fmt::print(fmt::fg(fmt::color::red), "\nTRANSITION RESULT ERROR");
            fmt::print("Code={}\n", errorTransition.code());
            fmt::print("Message={}\n", errorTransition.message());
            fmt::print("Data={}\n", errorTransition.data());
        }

        obj["proof"] = {{"rootTreeProof", waitStateRes.proof().signature_llmq_hash()},{"storeTreeProof", waitStateRes.proof().signature()}}; //TODO: Not sure if these are the right elements of the proof
        obj["metadata"] = {{"height", waitStateRes.metadata().height()}, {"coreChainLockedHeight", waitStateRes.metadata().core_chain_locked_height()}};

        return 0;
    }

    int32_t platform::identity(void)
    {
        nlohmann::json obj{};
        std::time_t start, end;

        grpc::ClientContext ctx;
        Dash::GetIdentityRequest identityReq;
        Dash::GetIdentityResponse identityRes;

        start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        connection->stub->getIdentity(&ctx, identityReq, &identityRes);

        end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        if(!(identityRes.identity().empty())) {
            fmt::print(fmt::fg(fmt::color::green), "\nIDENTITY: {}\n", identityRes.identity());
        }

        if(identityRes.has_metadata()) {
            PrintMetadataMsg(identityRes.metadata());
        }

        obj["identity"] = identityRes.identity();
        obj["metadata"] = {{"height", identityRes.metadata().height()}, {"coreChainLockedHeight", identityRes.metadata().core_chain_locked_height()}};

        return 0;
    }

    int32_t platform::data_contract(void)
    {
        nlohmann::json obj{};
        std::time_t start, end;

        grpc::ClientContext* ctx;
        Dash::GetDataContractRequest contractReq;
        Dash::GetDataContractResponse contractRes;

        start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        connection->stub->getDataContract(ctx, contractReq, &contractRes);

        end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        if(!(contractRes.data_contract().empty())) {
            fmt::print(fmt::fg(fmt::color::green), "\nCONTRACT: {}\n", contractRes.data_contract());
        }

        if(contractRes.has_metadata()) {
            PrintMetadataMsg(contractRes.metadata());
        }

        obj["dataContract"] = contractRes.data_contract();
        obj["metadata"] = {{"height", contractRes.metadata().height()}, {"coreChainLockedHeight", contractRes.metadata().core_chain_locked_height()}};

        return 0;
    }

    int32_t platform::documents(void)
    {
        nlohmann::json obj{};
        std::time_t start, end;

        grpc::ClientContext ctx;
        Dash::GetDocumentsRequest documentReq;
        Dash::GetDocumentsResponse documentRes;

        start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        connection->stub->getDocuments(&ctx, documentReq, &documentRes);

        end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        if(documentRes.documents_size() > 0) {
            fmt::print(fmt::fg(fmt::color::green), "\nDOCUMENT SIZE: {} byte\n", documentRes.documents_size());
        }

        if(documentRes.has_metadata()) {
            PrintMetadataMsg(documentRes.metadata());
        }

        obj["dataContract"] = {documentRes.documents()};
        obj["metadata"] = {{"height", documentRes.metadata().height()}, {"coreChainLockedHeight", documentRes.metadata().core_chain_locked_height()}};

        return 0;
    }

    int32_t platform::identities_by_public_key_hashes(void)
    {
        nlohmann::json obj{};
        std::time_t start, end;

        grpc::ClientContext ctx;
        Dash::GetIdentitiesByPublicKeyHashesRequest identitiesReq;
        Dash::GetIdentitiesByPublicKeyHashesResponse identitiesRes;

        start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        connection->stub->getIdentitiesByPublicKeyHashes(&ctx, identitiesReq, &identitiesRes);

        end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        if(identitiesRes.identities().size() > 0) {
            std::string sIdentities = "";
            for(auto identity: identitiesRes.identities()) {
                sIdentities += identity;
            }

            fmt::print(fmt::fg(fmt::color::green), "\nIDENTITIES: {}\n", sIdentities);
        }

        if(identitiesRes.identities_size() > 0) {
            fmt::print(fmt::fg(fmt::color::green), "\nIDENTITY SIZE: {} byte\n", identitiesRes.identities_size());
        }

        if(identitiesRes.has_metadata()) {
            PrintMetadataMsg(identitiesRes.metadata());
        }

        obj["identities"] = {identitiesRes.identities()};
        obj["metadata"] = {{"height", identitiesRes.metadata().height()}, {"coreChainLockedHeight", identitiesRes.metadata().core_chain_locked_height()}};

        return 0;
    }

    int32_t platform::identity_ids_by_public_key_hashes(void)
    {
        nlohmann::json obj{};
        std::time_t start, end;

        grpc::ClientContext ctx;
        Dash::GetIdentityIdsByPublicKeyHashesRequest identityIdReq;
        Dash::GetIdentityIdsByPublicKeyHashesResponse identityIdRes;

        start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        connection->stub->getIdentityIdsByPublicKeyHashes(&ctx, identityIdReq, &identityIdRes);

        end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        if(!(identityIdRes.identity_ids().empty())) {
            std::string sIds = "";
            for(auto identityId: identityIdRes.identity_ids()) {
                sIds += identityId;
            }
            fmt::print(fmt::fg(fmt::color::green), "\nIDENTITIES: {}\n", sIds);
        }

        if(identityIdRes.identity_ids_size() > 0) {
            fmt::print(fmt::fg(fmt::color::green), "\nIDENTITY SIZE: {} byte\n", identityIdRes.identity_ids_size());
        }

        if(identityIdRes.has_metadata()) {
            PrintMetadataMsg(identityIdRes.metadata());
        }

        obj["identityIds"] = {identityIdRes.identity_ids()};
        obj["metadata"] = {{"height", identityIdRes.metadata().height()}, {"coreChainLockedHeight", identityIdRes.metadata().core_chain_locked_height()}};

        return 0;
    }

    int32_t platform::consensus_params(void)
    {
        nlohmann::json obj{};
        std::time_t start, end;
        
        grpc::ClientContext ctx;
        Dash::GetConsensusParamsRequest consensusReq;
        Dash::GetConsensusParamsResponse consensusRes;

        start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        connection->stub->getConsensusParams(&ctx, consensusReq, &consensusRes);

        end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        if(consensusRes.has_block()) {
            auto conBlock{consensusRes.block()};
            fmt::print(fmt::fg(fmt::color::green), "\nCONSENSUS BLOCK\n");
            fmt::print("Max-bytes={}\n", conBlock.max_bytes());
            fmt::print("Max-gas={}\n", conBlock.max_gas());
            fmt::print("Time-iota-ms={}\n", conBlock.time_iota_ms());
        }

        if(consensusRes.has_evidence()) {
            auto conEvidence{consensusRes.evidence()};
            fmt::print(fmt::fg(fmt::color::green), "\nCONSENSUS EVIDENCE\n");
            fmt::print("Max-age-num-blocks={}\n", conEvidence.max_age_num_blocks());
            fmt::print("Max-age-duration={}\n", conEvidence.max_age_duration());
            fmt::print("Max-bytes={}\n", conEvidence.max_bytes());
        }

        return 0;
    }

} // namespace dashrpc
