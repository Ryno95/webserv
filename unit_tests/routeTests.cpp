#include <criterion/criterion.h>

#include "../includes/config/LocationConfig.hpp"
#include "../includes/config/ServerConfig.hpp"
#include "../includes/config/HostConfig.hpp"
#include "../includes/config/ConfigFileParser.hpp"
#include "../includes/Host.hpp"

#include <vector>
#include <iostream>

using namespace Webserver;

ConfigFileParser parser("unit_tests/default.config");

Test(RouteTests, NonExistingHost)
{
	std::vector<ServerConfig> config = parser.parse();
	Host host = Host::determine(config[0], "asdfasdf", "/");

	cr_expect(host.getName() == "main_server.nl");
	cr_expect(host.getRoot() == "root");
}

Test(RouteTests, SpecificDefaultHost)
{
	std::vector<ServerConfig> config = parser.parse();
	Host host = Host::determine(config[0], "main_server.nl", "/upload/images/123.png");

	cr_expect(host.getName() == "main_server.nl");
	cr_expect(host.getRoot() == "/data/upload/");
}

Test(RouteTests, SpecificHostReroute)
{
	std::vector<ServerConfig> config = parser.parse();
	Host host = Host::determine(config[0], "cgi_handler", "/root/cgi-bin/add.py");

	cr_expect(host.getName() == "cgi_handler");
	cr_expect(host.getRoot() == "/root/cgi/");
}

Test(RouteTests, SpecificHostRedirect)
{
	std::vector<ServerConfig> config = parser.parse();
	Host host = Host::determine(config[0], "cgi_handler", "/googlesearch/search=animals&kind=cats");

	cr_expect(host.getName() == "cgi_handler");
	cr_expect(host.getRoot() == "www.google.com");
}

Test(RouteTests, SpecificHostUpload)
{
	std::vector<ServerConfig> config = parser.parse();
	Host host = Host::determine(config[0], "cgi_handler", "/images/cool_images/sprite.jpg");

	cr_expect(host.getName() == "cgi_handler");
	cr_expect(host.getRoot() == "/root/data/images");
}
