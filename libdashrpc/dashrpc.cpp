#define INCLUDE_DASHRPC_EXTERNAL
#include "dashrpc.hpp"

#include "fmt/color.h"
#include "fmt/core.h"
#include "grpcpp/grpcpp.h"
#include <fstream>

#include "../libdashprotos/core.grpc.pb.h"
#include "../libdashprotos/core.pb.h"


namespace dashrpc
{

nlohmann::json read_json(std::string const &path)
{
    std::ifstream json_file{path};
    nlohmann::json json_text{};
    if (json_file.is_open())
    {
        json_file >> json_text;
        json_file.close();
    }
    else
    {
        throw std::logic_error{"File cannot be opened at the specifed location."};
    }
    return json_text;
}

std::string grpc_version(void)
{
    return grpc::Version();
}

void grpc_dash_layer(void)
{
    auto channel =
        grpc::CreateChannel("http://seed-1.testnet.networks.dash.org:3010", grpc::InsecureChannelCredentials());
    auto stub = org::dash::platform::dapi::v0::Core::NewStub(channel);

    org::dash::platform::dapi::v0::GetStatusRequest req;
    org::dash::platform::dapi::v0::GetStatusResponse res;
    grpc::ClientContext ctx;

    stub->getStatus(&ctx, req, &res);


    if (res.has_version())
    {
        auto version{res.version()};
        fmt::print(fmt::fg(fmt::color::green), "\nVERSION\n");
        fmt::print("protocol={}\n", version.protocol());
        fmt::print("software={}\n", version.software());
        fmt::print("agent={}\n", version.agent());
    }

    if (res.has_time())
    {
        auto time{res.time()};
        fmt::print(fmt::fg(fmt::color::green), "\nTIME\n");
        fmt::print("now={}\n", time.now());
        fmt::print("offset={}\n", time.offset());
        fmt::print("median={}\n", time.median());
    }

    if (res.has_chain())
    {
        auto chain{res.chain()};
        fmt::print(fmt::fg(fmt::color::green), "\nCHAIN\n");
        fmt::print("name={}\n", chain.name());
        fmt::print("header_count={}\n", chain.headers_count());
        fmt::print("block_count={}\n", chain.blocks_count());
        fmt::print("best_block_hash={}\n", chain.best_block_hash());
        fmt::print("difficulty={}\n", chain.difficulty());
        fmt::print("chain_work={}\n", chain.chain_work());
        fmt::print("is_synced={}\n", chain.is_synced());
        fmt::print("sync_progess={}\n", chain.sync_progress());
    }

    if (res.has_masternode())
    {
        auto masternode{res.masternode()};
        fmt::print(fmt::fg(fmt::color::green), "\nMASTERNODE\n");
        fmt::print("status={}\n", masternode.status());
        fmt::print("protxhash={}\n", masternode.pro_tx_hash());
        fmt::print("pose_penalty={}\n", masternode.pose_penalty());
        fmt::print("is_synced={}\n", masternode.is_synced());
        fmt::print("sync_process={}\n", masternode.sync_progress());
    }

    if (res.has_network())
    {
        auto network{res.network()};
        fmt::print(fmt::fg(fmt::color::green), "\nNETWORK\n");
        fmt::print("peer_count={}\n", network.peers_count());
        fmt::print("fee_relay={}\n", network.fee().relay());
        fmt::print("fee_incremental={}\n", network.fee().incremental());
    }
}

} // namespace dashrpc
