#include <iostream>
#include <Webserv.hpp>

int main()
{
	Webserv test(8080, "Testserver");

	test.run();
	return (0);
}
