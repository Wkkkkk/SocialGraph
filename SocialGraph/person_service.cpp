#include "person_service.h"

PersonService::~PersonService()
{
	std::cout << "~PersonService" << std::endl;
}

void PersonService::add_person(Person person)
{
	population.insert(person);
}

std::optional<Person> PersonService::find_person(int id)
{
	for (const auto& person : population) {
		if (person.id == id) return person;
	}

	return {};
}
