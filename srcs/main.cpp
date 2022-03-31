#include <iostream>
#include <Webserv.hpp>

int main()
{
	Webserv test(4242, "Testserver");

	test.run();
	return (0);
}
