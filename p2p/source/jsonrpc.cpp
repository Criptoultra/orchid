/* Orchid - WebRTC P2P VPN Market (on Ethereum)
 * Copyright (C) 2017-2019  The Orchid Authors
*/

/* GNU Affero General Public License, Version 3 {{{ */
/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.

 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/
/* }}} */


#include <json/json.h>

#include "error.hpp"
#include "http.hpp"
#include "jsonrpc.hpp"
#include "trace.hpp"

namespace orc {

task<std::string> Endpoint::operator ()(const std::string &method, const std::vector<std::string> &args) {
    Json::Value root;
    root["jsonrpc"] = "2.0";
    root["method"] = method;
    root["id"] = "";

    Json::Value params;
    for (size_t i(0); i != args.size(); ++i)
        params[Json::ArrayIndex(i)] = args[i];
    root["params"] = std::move(params);

    Json::FastWriter writer;
    auto body(co_await Request("POST", uri_, {{"content-type", "application/json"}}, writer.write(root)));
    Log() << "[[ " << body << " ]]" << std::endl;

    Json::Value result;
    Json::Reader reader;
    _assert(reader.parse(std::move(body), result, false));

    _assert(result["jsonrpc"] == "2.0");
    _assert(result["id"] == "");
    co_return result["result"].asString();
}

}