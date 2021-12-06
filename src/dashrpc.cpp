#include "fmt/color.h"
#include "fmt/core.h"

#include "nlohmann/json.hpp"

#include "grpcpp/grpcpp.h"

#include <fstream>

#include "dashrpc.hpp"

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