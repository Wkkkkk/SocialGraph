#include <iostream>

#include <absl/strings/str_split.h>
#include <boost/beast/version.hpp>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "singleton.h"
#include "request_handler.h"
#include "person.h"

using namespace SocialGraph;
using json = nlohmann::json;

int RequestHandler::retrive_query_content_from_request_target(const http::request<http::string_body>& request)
{
    int person_id;

    std::string target = request.target().to_string();
    std::vector<std::string> parts = absl::StrSplit(target, "?");
    if (parts.size() == 2) {
        std::vector<std::string> all_parameters = absl::StrSplit(parts[1], "&");

        for (std::string one_parameter : all_parameters) {
            std::vector<std::string> parameter_pair = absl::StrSplit(one_parameter, "=");
            if (parameter_pair.size() == 2) {
                if (parameter_pair[0] == "person_id") {
                    person_id = std::stoi(parameter_pair[1]);

                    break;
                }
            }
        }
    }

    return person_id;
}

RequestHandler::RequestHandler() {}

RequestHandler::result_pair
RequestHandler::handle(const http::request<http::string_body>& request)
{
    // Analyze the query content
    int person_id = retrive_query_content_from_request_target(request);
    if (person_id == 0) {
        return { RequestHandler::STATUS::illegal_request, request.target().to_string() };
    }

    // Get a rpc client
    auto& client = singleton::get_instance<rpc::client>("127.0.0.1", 9000);

    // Search for matched documents
    auto ids = client.call("usergraph_service_find_friends", person_id).as<std::vector<int>>();
    if (ids.empty()) {
        return { RequestHandler::STATUS::not_found, std::to_string(person_id) };
    }

    auto friends = client.call("usergraph_service_find_people", ids).as<std::vector<Person>>();
    if (friends.empty()) {
        return { RequestHandler::STATUS::not_found, std::to_string(person_id) };
    }

    // generate response json
    json j;
    for (const auto& person : friends) {
        json doc;
        doc["id"] = person.id;
        doc["name"] = person.name;
        doc["link"] = "https://social.com/" + person.name;

        j.push_back(doc);
    }
    std::string result = j.dump();

    return { RequestHandler::STATUS::ok, result };
}
