#pragma once

#include <poll.h>

#include <iostream>
#include <string>
#include <vector>

#include <Client.hpp>
#include <config/ServerConfig.hpp>
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

		pollfd&					_listener;
		std::vector<Client*>	_clients;
};
