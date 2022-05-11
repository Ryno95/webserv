#pragma once

#include <vector>

#include <Webserv.hpp>

namespace Webserver
{
	class ServerHandler
	{
	public:
		static void addServer(const ServerConfig& config);
		static void run();
		static void stop();

		static const std::vector<Webserv*>& getServers();

	private:
		static std::vector<Webserv*> _servers;
	};
}
