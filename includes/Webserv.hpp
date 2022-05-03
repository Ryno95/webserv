#pragma once

#include <poll.h>

#include <iostream>
#include <string>
#include <vector>

#include <Client.hpp>

#define BACKLOG_AMOUNT 42

class Webserv
{
	public:
		Webserv(uint port, std::string name);
		~Webserv();

		void run();

	private:
		void setupSocket();
		void handleListener();
		void handleClients();
		void handleTimeout();
		void removeClient(int index);

		// const ServerConfig _config;
		uint				_port;
		int					_listenFd;
		std::vector<pollfd>	_fds;
		std::vector<Client>	_clients;
		std::string			_name;
};
