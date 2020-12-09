#pragma once
#include <optional>

#include "person.h"

class PersonService
{
public:
	PersonService() = default;
	~PersonService();

	void add_person(Person person);

	std::optional<Person> find_person(int id);

private:
	std::set<Person> population;
};

