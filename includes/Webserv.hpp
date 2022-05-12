#pragma once

#include <poll.h>

#include <iostream>
#include <string>
#include <vector>

#include <Router.hpp>
#include <Client.hpp>
#include <config/ServerConfig.hpp>
#include <PollHandler.hpp>

namespace Webserver
{
	class Webserv
	{
		public:
			Webserv(const ServerConfig& config);
			~Webserv();

			void handle();
			const ServerConfig& getConfig() const;
			const Router& getRouter() const;

		private:
			void setup();
			void handleListener();
			void handleClients();
			void removeClient(int index);

			const ServerConfig& _config;
			const Router _router;

			int						_listenerFd;
			std::vector<Client*>	_clients;
	};
}
