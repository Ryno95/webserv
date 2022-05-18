#include <criterion/criterion.h>

#include "../includes/config/LocationConfig.hpp"
#include "../includes/config/ServerConfig.hpp"
#include "../includes/config/HostConfig.hpp"
#include "../includes/config/ConfigFileParser.hpp"
#include "../includes/Router.hpp"
// #include "../includes/Request.hpp"

#include <vector>
#include <iostream>

using namespace Webserver;

ConfigFileParser parser("unit_tests/default.config");

Test(RouteTests, NonExistingHost)
{
	std::vector<ServerConfig> configs = parser.parse();
	Router router(configs[0]);
	HostConfig host = router.getHost("asdfasdf", "/");
	cr_expect(host.names[0] == "main_server.nl");
	cr_expect(host.names[1] == "main_server.com");
	cr_expect(host.root == "root");
}

Test(RouteTests, SpecificDefaultHost)
{
	std::vector<ServerConfig> configs = parser.parse();
	Router router(configs[0]);
	HostConfig host = router.getHost("main_server.nl", "/upload/images/123.png");
	cr_expect(host.names[0] == "main_server.nl");
	cr_expect(host.names[1] == "main_server.com");
	cr_expect(host.root == "/data/upload/");
}

Test(RouteTests, SpecificHostReroute)
{
	std::vector<ServerConfig> configs = parser.parse();
	Router router(configs[0]);
	HostConfig host = router.getHost("cgi_handler", "/root/cgi-bin/add.py");
	cr_expect(host.names[0] == "cgi_handler");
	cr_expect(host.root == "/root/cgi/");
}

Test(RouteTests, SpecificHostRedirect)
{
	std::vector<ServerConfig> configs = parser.parse();
	Router router(configs[0]);
	HostConfig host = router.getHost("cgi_handler", "/googlesearch/search=animals&kind=cats");
	cr_expect(host.names[0] == "cgi_handler");
	cr_expect(host.root == "www.google.com");
}

Test(RouteTests, SpecificHostUpload)
{
	std::vector<ServerConfig> configs = parser.parse();
	Router router(configs[0]);
	HostConfig host = router.getHost("cgi_handler", "/images/cool_images/sprite.jpg");
	cr_expect(host.names[0] == "cgi_handler");
	cr_expect(host.root == "/root/data/images");
}
