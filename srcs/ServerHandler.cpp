#include <ServerHandler.hpp>

namespace Webserver
{
	std::vector<Webserv*>	ServerHandler::_servers;

	void ServerHandler::addServer(const ServerConfig& config)
	{
		_servers.push_back(new Webserv(config)); // no delete of this heap yet
	}

	void ServerHandler::run()
	{
		while (true)
		{
			PollHandler::checkPoll();

			for (size_t i = 0; i < _servers.size(); i++)
			{
				_servers[i]->handle();
			}
		}
	}
}
