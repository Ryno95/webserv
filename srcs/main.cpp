#include <iostream>

#include <Webserv.hpp>
#include <Logger.hpp>
#include <config/ConfigFileParser.hpp>

namespace Webserver
{
	int run(int argc, char** argv)
	{
		try
		{
			std::string path;
			if (argc > 1)
				path = argv[1];
			else
				path = "config/default.config";

			ConfigFileParser parser(path);
			std::vector<ServerConfig>& configs = parser.parse();

			for (size_t i = 0; i < configs.size(); i++)
			{
				PollHandler::addServer(configs[i]);
			}
			PollHandler::run();
		}
		catch(const std::exception& e)
		{
			ERROR(e.what());
			return 1;
		}

		return 0;
	}
}

int main(int argc, char **argv)
{
	return Webserver::run(argc, argv);
}
