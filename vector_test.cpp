#include <vector>
#include <iostream>
#include <string>

struct Person
{
	Person(std::string n, int a) : name(n), age(a) {}
	Person() {};

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

	delete person;

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

void PersonsAfterAddedToVector()
{
	std::vector<Person> persons;

	persons.push_back(Person());
	Person& pascal = persons.front();

	pascal.name = "Pascal";
	pascal.age = 26;

	std::cout << "Name: " << persons.front().name << " age: " << persons.front().age << std::endl;
}

int main()
{
	PersonsByValue();
	PersonsByAddress();
	PersonsByHeap();
	PersonsAfterAddedToVector();
}
