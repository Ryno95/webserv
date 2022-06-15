#include <iostream>
#include <fstream>
#include <Webserv.hpp>
#include <Logger.hpp>

#include <TickHandler.hpp>
#include <TimeoutHandler.hpp>
#include <config/Parser.hpp>
#include <config/AppConfigParser.hpp>

namespace Webserver
{

	void loop()
	{
		while (true)
		{
			PollHandler::get().update();
			TickHandler::get().update();
			TimeoutHandler::get().update();
		}
	}

	AppConfig* parseConfig(const std::string& path)
	{
		try
		{
			std::ifstream fstream(path);
			if (fstream.fail())
				throw FileNotFoundException(path);
			return Parser<AppConfigParser>(fstream).parse();
		}
		catch(const std::exception& e)
		{
			throw std::runtime_error(std::string("In config file: ") + e.what());
		}
	}

	int run(int argc, char** argv)
	{
		std::vector<Webserv*> servers;

		try
		{
			std::string path;
			if (argc > 1)
				path = argv[1];
			else
				path = "config/default.config";
			AppConfig* config = parseConfig(path);
			Webserv::config(config);

			for (size_t i = 0; i < config->getChildren().size(); i++)
			{
				servers.push_back(new Webserv(*config->getChildren()[i]));
			}

			loop();
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
