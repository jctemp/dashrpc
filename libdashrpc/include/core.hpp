#pragma once

#include <memory>
#include <string>

namespace dashrpc
{

/**
 * @brief core is a wrapper class to handle all grpc implementations
 * details to retrieve any information concerning layer 1. The
 * naming of the member functions relate to the protocol buffer
 * files.
 *
 */
class core
{
private:
    struct connection;
    static std::unique_ptr<connection> connection;

public:
    core() = delete;
    core(const core &) = delete;
    ~core() = delete;

    /**
     * @brief initialising the client to be ready to make any connections to the
     * specified address with port
     *
     * @param address IPv4 or domain name
     * @param port valid number in range [0,65535]
     */
    static void init(std::string address = "seed-1.testnet.networks.dash.org", uint16_t port = 3010);

    static int32_t status(void);
    static int32_t block(void);
    static int32_t broadcast_transaction(void);
    static int32_t transaction(void);
    static int32_t estimated_transaction_fee(void);
    static int32_t subscribe_to_block_header_with_chain_locks(void);
    static int32_t subscribe_to_transaction_with_proofs(void);
};

} // namespace dashrpc
