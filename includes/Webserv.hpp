#pragma once

#include <poll.h>

#include <iostream>
#include <string>
#include <vector>

#include <Client.hpp>
#include <ServerConfig.hpp>
#include <ServerHandler.hpp>

class Webserv
{
	public:
		Webserv(const ServerConfig& config);
		~Webserv();

		void handle();

	private:
		void setup();
		void handleListener();
		void handleClients();
		void removeClient(int index);

		const ServerConfig& _config;

		int						_listenerFd;
		std::vector<Client*>	_clients;
};
