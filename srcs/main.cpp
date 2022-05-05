#include <iostream>
#include <Webserv.hpp>
#include <Logger.hpp>
#include <ConfigFileParser.hpp>

int main(int argc, char **argv)
{
	std::string path;
	if (argc > 1)
	{
		WARN("Using custom config files is not yet tested.");
		path = argv[1];
	}
	else
		path = "config/default.config";

	ConfigFileParser parser(path);
	std::vector<ServerConfig>& configs = parser.parse();

	for (size_t i = 0; i < configs.size(); i++)
	{
		ServerHandler::addServer(configs[i]);
	}
	ServerHandler::run();

	return 0;
}
