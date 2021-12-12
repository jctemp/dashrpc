#include "fmt/color.h"
#include "fmt/core.h"
#include "grpcpp/grpcpp.h"
#include "nlohmann/json.hpp"
#include <fstream>

#include "dashrpc.hpp"

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
}

} // namespace dashrpc
