#pragma once

#include <poll.h>

#include <iostream>
#include <string>
#include <vector>

#define BACKLOG_AMOUNT 42
#define BUFFER_SIZE 1024
#define DISCONNECT 17

// Closing of fds will take place in te Client class
class Webserv
{
	public:
		Webserv(uint port, std::string name);
		~Webserv();

		void run();

	private:
		void setupSocket();

		uint				_port;
		int					_listenFd;
		std::vector<pollfd>	_fds;
		char				*_buffer;
		std::string			_name;
};
