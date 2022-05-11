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

	private:
		static std::vector<Webserv*> _servers;
	};
}
