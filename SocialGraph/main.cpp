// SocialGraph.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <memory>

#include <rpc/server.h>

#include "person_service.h"
#include "usergraph_service.h"

int main()
{
    // Creating a server that listens on port 8080
    rpc::server srv(9000);

    // Our service
    LookupService lookup_service;
    for (int i = 0; i < 10; i++) {
        auto person_service = std::make_shared<PersonService>();
        for (int j = 0; j < 10; j++) {
            person_service->add_person(generate_random_person(i * 10 + j));
        }
        lookup_service.add_person_service(person_service);
    }

    UserGraphService usergraph_service(lookup_service);

    srv.bind("usergraph_service_find_friends",
        [&usergraph_service](int id) -> std::vector<int> {
            std::vector<int> friends;
            auto person = usergraph_service.find_person(id);
            if (person.has_value()) {
                auto friends_set = person.value().friend_ids;
                friends.insert(friends.end(), friends_set.begin(), friends_set.end());
            }
            return friends;
        });

    srv.bind("usergraph_service_find_people", 
        [&usergraph_service](std::vector<int> ids) -> std::vector<Person> {
            return usergraph_service.find_people(ids);
        });

    srv.bind("usergraph_service_shortest_path", 
        [&usergraph_service](int source_id, int dest_id) -> std::vector<int> {
        return usergraph_service.shortest_path(source_id, dest_id);
        });

    // Run the server loop.
    srv.run();

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
