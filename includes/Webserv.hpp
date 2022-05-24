#pragma once

#include <poll.h>

#include <iostream>
#include <string>
#include <vector>

#include <Client.hpp>
#include <config/ServerConfig.hpp>
#include <IPollable.hpp>
#include <PollHandler.hpp>

namespace Webserver
{

	class Client;

	class Webserv : public IPollableTickable
	{
		public:
			Webserv(const ServerConfig& config);
			~Webserv();

			void readHandler();
			void writeHandler();
			void tick();
			const ServerConfig& getConfig() const;
			void checkTimeout() const;
			void removeClients();

		private:
			void setup();
			void handleListener();

			const ServerConfig& _config;

			int						_listenerFd;
			std::vector<Client*>	_clients;
	};
}
