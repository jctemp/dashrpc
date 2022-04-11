#pragma once

#include <memory>
#include <string>

namespace dashrpc
{

/**
 * @brief platform is a wrapper class to handle all grpc implementations
 * details to retrieve any information concerning layer 2. The naming
 * of the member functions relate to the protocol buffer files.
 *
 */
class platform
{
public:
    /**
     * @brief initialising the client to be ready to make any connections to the
     * specified address with port
     *
     * @param address IPv4 or domain name
     * @param port valid number in range [0,65535]
     */
    static bool init(std::string address = "seed-1.testnet.networks.dash.org", uint16_t port = 3010);

    static int32_t broadcast_state_transition(void);
    static int32_t identity(void);
    static int32_t data_contract(void);
    static int32_t documents(void);
    static int32_t identities_by_public_key_hashes(void);
    static int32_t identity_ids_by_public_key_hashes(void);
    static int32_t wait_for_state_transition_result(void);
    static int32_t consensus_params(void);

    platform() = delete;
    platform(const platform &) = delete;
    ~platform() = delete;

    struct connection;
    static std::unique_ptr<connection> connection;
};

} // namespace dashrpc
