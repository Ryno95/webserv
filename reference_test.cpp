#include <iostream>
#include <string>

struct Person
{
	Person(std::string n, int a) : name(n), age(a) {}

	std::string name;
	int age;
};


/*
	This is actually quite dangerous, because it can mess everything up!
	As shown below:
*/
void function(Person &person)
{
	person = Person("Nobody", person.age);
}

void pointer(Person *person)
{
	person->age = 99;
	person = nullptr;
}

void pointer(Person const * person)
{
	person->age = 25;
	person = nullptr;
}

void pointer(Person * const person)
{
	person->age = 25;
	person = nullptr;
}

void protectedFunction(Person const& person)
{
	// person = Person("Nobody", 0); does not compile
	// person.age++;
}

int main()
{
	Person pascal("Pascal", 26);

	// pointer(&pascal);
	protectedFunction(pascal);
	// function(pascal);
	std::cout << "Name: " << pascal.name << " age: " << pascal.age << std::endl;
}
