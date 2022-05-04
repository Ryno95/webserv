#pragma once

#include <vector>

class Webserv;

#include <Webserv.hpp>
#include <config/ServerConfig.hpp>

class ServerHandler
{
public:
	static void run();
	static pollfd& addPollfd();
	static void removePollfd(const pollfd& fd);

	static void addServer(const ServerConfig& config);

private:
	static std::vector<Webserv*>	_servers;
	static std::vector<pollfd>		_fds;

};
