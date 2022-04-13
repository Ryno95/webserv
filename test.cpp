#include <vector>
#include <iostream>
#include <string>

struct Person
{
	Person(std::string n, int a) : name(n), age(a) {}

	std::string name;
	int age;
};

void PersonsByHeap()
{
	std::vector<Person *> persons;
	Person *pascal = new Person("Pascal", 26);

	persons.push_back(pascal);

	Person *person = persons.front();

	person->age += 1;

	std::cout << "Name: " << pascal->name << " age: " << pascal->age << std::endl;

	delete pascal;

	// Person *heapUseAfterFree = persons.front();
	// heapUseAfterFree->age = -1;
}

void PersonsByAddress()
{
	std::vector<Person *> persons;
	Person pascal("Pascal", 26);

	persons.push_back(&pascal);
	Person *person = persons.front();
	person->age += 1;

	std::cout << "Name: " << pascal.name << " age: " << pascal.age << std::endl;
}

void PersonsByValue()
{
	std::vector<Person> persons;
	Person pascal("Pascal", 26);

	persons.push_back(pascal);
	Person person = persons.front();
	person.age += 1;

	std::cout << "Name: " << pascal.name << " age: " << pascal.age << std::endl;
}

int main()
{
	PersonsByValue();
	PersonsByAddress();
	PersonsByHeap();
}
