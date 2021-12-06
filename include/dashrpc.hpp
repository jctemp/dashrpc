#pragma once

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

std::string grpc_version(void);