#include <criterion/criterion.h>
#include "../includes/Uri.hpp"

using namespace Webserver;

Test(UriTests, AbsoluteValidTest1)
{
	Uri uri("http://google.com/homepage.html");

	cr_expect(uri.getResourcePath() == "/homepage.html");
	std::cout << uri.getHost();
	cr_expect(uri.getHost() == "google.com");
}
