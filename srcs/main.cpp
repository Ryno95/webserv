#include <iostream>
#include <Webserv.hpp>

int main()
{
	try
	{
		Webserv test(8080, "Testserver");
		test.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	return (0);
}
