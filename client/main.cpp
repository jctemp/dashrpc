#include <iostream>
#include <vector>

#include "rpc/client.h"
#include "rpc/rpc_error.h"

#include "nlohmann/json.hpp"

int main()
{
    try
    {
        /*
         * RPC LIB DEMO
        */
        rpc::client c("127.0.0.1", 20002);
        auto state = c.get_connection_state();
        switch (state)
        {
        case rpc::client::connection_state::initial:
            std::cout << "initial";
            break;
        case rpc::client::connection_state::connected:
            std::cout << "connected";
            break;
        case rpc::client::connection_state::disconnected:
            std::cout << "disconnected";
            break;
        case rpc::client::connection_state::reset:
            std::cout << "reset";
            break;
        }
        std::cout << std::endl;

        /*
         * JSON LIB DEMO
        */
        nlohmann::json obj;
        obj.push_back("foo");
        obj.push_back("bar");
        obj.push_back(42);
        obj.push_back(true);

        for (nlohmann::json::iterator it = obj.begin(); it != obj.end(); ++it)
            std::cout << *it << std::endl;

        std::cout << obj << std::endl;
    }
    catch (rpc::rpc_error &e)
    {
        std::cout << std::endl << e.what() << std::endl;
        std::cout << "in function " << e.get_function_name() << ": ";

        using err_t = std::tuple<int, std::string>;
        auto err = e.get_error().as<err_t>();
        std::cout << "[error " << std::get<0>(err) << "]: " << std::get<1>(err) << std::endl;
        return 1;
    }
}
