#pragma once
#include <map>

#include <optional>

#include "lookup_service.h"

class UserGraphService
{
public:
	UserGraphService(LookupService lookup_service);

	std::optional<Person> find_person(int id);
	std::vector<Person> find_people(std::vector<int> ids);
	std::vector<int> shortest_path(int source_id, int dest_id);

private:
	std::map<int, int> _shortest_path(int source_id, int dest_id);

	LookupService lookup_service_;
};

