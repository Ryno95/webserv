#pragma once

#include <poll.h>

#include <iostream>
#include <string>
#include <vector>

#define BACKLOG_AMOUNT 42
// Closing of fds will take place in te Client class
class Webserv
{
	public:
		Webserv(uint port, std::string name);
		~Webserv();


	private:
		void setupSocket();

		uint				_port;
		std::vector<pollfd>	_fds;
		std::string			_name;
		
};
