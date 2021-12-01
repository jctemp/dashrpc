#include <string>
#include <vector>

#include "rpc_utils.hpp"

struct rpc_target
{
    std::string host;
    std::string port;
    std::string user;
    std::string password;

    rpc_target(std::string const &host, std::string const &port, std::string const &user, std::string const &password)
        : host{host}, port{port}, user{user}, password{password}
    {
    }

    rpc_target() : host{"127.0.0.1"}, port{"8080"}, user{"user"}, password{"password"}
    {
    }
};

struct rpc_request
{
    rpc_target target;
    std::string method;
    std::vector<std::string> params;
};
