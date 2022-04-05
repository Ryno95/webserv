#include <iostream>
#include <Webserv.hpp>

int main()
{
	Webserv test(80, "Testserver");

	test.run();
	return (0);
}
