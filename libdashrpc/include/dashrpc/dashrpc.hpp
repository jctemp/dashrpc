/*
   ___  ___   ______ _____  ___  _____
  / _ \/ _ | / __/ // / _ \/ _ \/ ___/   DashRPC Wrapper
 / // / __ |_\ \/ _  / , _/ ___/ /__     version 0.0.0
/____/_/ |_/___/_//_/_/|_/_/   \___/     https://github.com/jamie-prog/dashrpc

Licensed under GPL-3.0 License <https://opensource.org/licenses/GPL-3.0>.
SPDX-License-Identifier: GPL-3.0

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

 */

#ifndef INCLUDE_DASHRPC_HPP_
#define INCLUDE_DASHRPC_HPP_

#define DASHRPC_VERSION_MAJOR 0
#define DASHRPC_VERSION_MINOR 2
#define DASHRPC_VERSION_PATCH 0

#include <nlohmann/json.hpp>
#include "core.hpp"
#include "platform.hpp"

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

} // namespace dashrpc

#endif // INCLUDE_DASHRPC_HPP_
