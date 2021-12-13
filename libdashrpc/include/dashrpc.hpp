/*
   ___  ___   ______ _____  ___  _____
  / _ \/ _ | / __/ // / _ \/ _ \/ ___/   Dash RPC Wrapper
 / // / __ |_\ \/ _  / , _/ ___/ /__     version 0.0.0
/____/_/ |_/___/_//_/_/|_/_/   \___/     https://github.com/jamie-prog/dashrpc

 */

#ifndef INCLUDE_DASHRPC_HPP_
#define INCLUDE_DASHRPC_HPP_

#ifdef INCLUDE_DASHRPC_EXTERNAL
#include "nlohmann/json.hpp"
#endif

namespace dashrpc
{

/**
 * @brief Reads from a json file and create a nlohmann::json object
 * Has to be included after the <nlohmann/json.hpp> library and <fstream>
 * library
 *
 * @param path to the desired json file
 * @return nlohmann::json with the json-file content
 * @throws std::logic_error if the file could not be read
 */
nlohmann::json read_json(std::string const &path);

/**
 * @brief returns the version of the used grpc version
 *
 * @return std::string with the version
 */
std::string grpc_version(void);

void grpc_dash_layer(void);

} // namespace dashrpc

#endif // INCLUDE_DASHRPC_HPP_
