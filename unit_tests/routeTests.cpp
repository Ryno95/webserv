#include <criterion/criterion.h>

// #include "../includes/config/LocationConfig.hpp"
// #include "../includes/config/ServerConfig.hpp"
// #include "../includes/config/HostConfig.hpp"
#include "../includes/config/Parser.hpp"
#include "../includes/config/AppConfig.hpp"
#include "../includes/config/AppConfigParser.hpp"
#include "../includes/Host.hpp"

#include <vector>
#include <iostream>

using namespace Webserver;


Test(RouteTests, NonExistingHost)
{
	std::ifstream stream("unit_tests/default.config");
	AppConfig* config = Parser<AppConfigParser>(stream).parse();
	Host host = Host::determine(*config->getChildren()[0], "asdfasdf", "/");

	cr_expect(host.getRoot() == "root");
}

Test(RouteTests, UploadLocation)
{
	std::ifstream stream("unit_tests/default.config");
	AppConfig* config = Parser<AppConfigParser>(stream).parse();
	Host host = Host::determine(*config->getChildren()[0], "main_server.nl", "/upload/images/123.png");

	cr_expect(host.getRoot() == "root/data/");
}

Test(RouteTests, SpecificHostReroute)
{
	std::ifstream stream("unit_tests/default.config");
	AppConfig* config = Parser<AppConfigParser>(stream).parse();
	Host host = Host::determine(*config->getChildren()[0], "cgi_handler", "/root/cgi-bin/add.py");

	cr_expect(host.getRoot() == "root/cgi-bin/");
}

Test(RouteTests, SpecificHostRedirect)
{
	std::ifstream stream("unit_tests/default.config");
	AppConfig* config = Parser<AppConfigParser>(stream).parse();
	Host host = Host::determine(*config->getChildren()[0], "google", "/googlesearch/search=animals&kind=cats");

	cr_expect(host.getRoot() == "www.google.com");
}

Test(RouteTests, SpecificHostUpload)
{
	std::ifstream stream("unit_tests/default.config");
	AppConfig* config = Parser<AppConfigParser>(stream).parse();
	Host host = Host::determine(*config->getChildren()[0], "cgi_handler", "/images/cool_images/sprite.jpg");

	cr_expect(host.getRoot() == "root/data/");
}
