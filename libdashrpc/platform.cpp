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

    void platform::PrintMetadataMsg(auto metaDataRes) {
        fmt::print(fmt::fg(fmt::color::green), "\nMETADATA TRANSITION RESULT\n");
        fmt::print("Last comitted platform state height={}\n", metaDataRes.height());
        fmt::print("Most recent ChainLock on the core chain{}\n", metaDataRes.coreChainLockedHeight())
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

        connection->stub->waitForStateTransitionResult(ctx, waitStateReq, &waitStateRes);

        if(waitStateRes.has_metadata()) {
            PrintMetadataMsg(waitStateRes.metadata());
        }

        if(waitStateRes.has_proof()) {
            PrintProofMsg(waitStateRes.proof());
        }

        if(waitStateRes.has_error()) {
            auto errorTransition{waitStateRes.error()};
            fmt::print(fmt::fg(fmt::color::red), "\nTRANSITION RESULT ERROR");
            fmt::print("Code={}\n", errorTransition.code());
            fmt::print("Message={}\n", errorTransition.message());
            fmt::print("Data={}\n", errorTransition.data());
        }

        return 0;
    }

    int32_t platform::identity(void)
    {
        grpc::ClientContext ctx;
        Dash::GetIdentityRequest identityReq;
        Dash::GetIdentityResponse identityRes;

        connection->stub->getIdentity(&ctx, identityReq, &identityRes);

        if(!(identityRes.identity().empty())) {
            fmt::print(fmt::fg(fmt::color::green), "\nIDENTITY: {}\n", identityRes.identity());
        }

        if(identityRes.has_metadata()) {
            PrintMetadataMsg(identityRes.metadata());
        }

        if(identityRes.has_proof()) {
            PrintProofMsg(identityRes.proof());
        }

        return 0;
    }

    int32_t platform::data_contract(void)
    {

        grpc::ClientContext* ctx;
        Dash::GetDataContractRequest contractReq;
        Dash::GetDataContractResponse contractRes;

        connection->stub->getDataContract(ctx, contractReq, &contractRes);

        if(!(contractRes.data_contract().empty())) {
            fmt::print(fmt::fg(fmt::color::green), "\nCONTRACT: {}\n", contractRes.data_contract());
        }

        if(contractRes.has_proof()) {
            PrintProofMsg(contractRes.proof());
        }

        if(contractRes.has_metadata()) {
            PrintMetadataMsg(contractRes.metadata());
        }

        return 0;
    }

    int32_t platform::documents(void)
    {
        grpc::ClientContext ctx;
        Dash::GetDocumentsRequest documentReq;
        Dash::GetDocumentsResponse documentRes;

        connection->stub->getDocuments(&ctx, documentReq, &documentRes);

        if(documentRes.documents_size() > 0) {
            fmt::print(fmt::fg(fmt::color::green), "\nDOCUMENT SIZE: {} byte\n", documentRes.documents_size());
        }

        if(documentRes.has_proof()) {
            PrintProofMsg(documentRes.proof());
        }

        if(documentRes.has_metadata()) {
            PrintMetadataMsg(documentRes.metadata());
        }

        return 0;
    }

    int32_t platform::identities_by_public_key_hashes(void)
    {
        grpc::ClientContext ctx;
        Dash::GetIdentitiesByPublicKeyHashesRequest identitiesReq;
        Dash::GetIdentitiesByPublicKeyHashesResponse identitiesRes;

        connection->stub->getIdentitiesByPublicKeyHashes(&ctx, identitiesReq, &identitiesRes);

        if(identitiesRes.identities().size() > 0) {
            fmt::print(fmt::fg(fmt::color::green), "\nIDENTITIES: {}\n", identitiesRes.identities());
        }

        if(identitiesRes.identities_size() > 0) {
            fmt::print(fmt::fg(fmt::color::green), "\nIDENTITY SIZE: {} byte\n", identitiesRes.identities_size());
        }

        if(identitiesRes.has_proof()) {
            PrintProofMsg(identitiesRes.proof());
        }

        if(identitiesRes.has_metadata()) {
            PrintMetadataMsg(identitiesRes.metadata());
        }

        return 0;
    }

    int32_t platform::identity_ids_by_public_key_hashes(void)
    {
        grpc::ClientContext ctx;
        Dash::GetIdentityIdsByPublicKeyHashesRequest identityIdReq;
        Dash::GetIdentityIdsByPublicKeyHashesResponse identityIdRes;

        connection->stub->getIdentityIdsByPublicKeyHashes(&ctx, identityIdReq, &identityIdRes);

        if(!(identityIdRes.identity_ids().empty())) {
            fmt::print(fmt::fg(fmt::color::green), "\nIDENTITIES: {}\n", identityIdRes.identity_ids());
        }

        if(identityIdRes.identity_ids_size() > 0) {
            fmt::print(fmt::fg(fmt::color::green), "\nIDENTITY SIZE: {} byte\n", identityIdRes.identity_ids_size());
        }

        if(identityIdRes.has_proof()) {
            PrintProofMsg(identityIdRes.proof());
        }

        if(identityIdRes.has_metadata()) {
            PrintMetadataMsg(identityIdRes.metadata());
        }

        return 0;
    }

    int32_t platform::consensus_params(void)
    {
        grpc::ClientContext ctx;
        Dash::GetConsensusParamsRequest consensusReq;
        Dash::GetConsensusParamsResponse consensusRes;

        connection->stub->getConsensusParams(&ctx, consensusReq, &consensusRes);

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
