#pragma once

#include <vector>

class Webserv;

#include <Webserv.hpp>
#include <config/ServerConfig.hpp>

class ServerHandler
{
public:
	static void run();
	static void addPollfd(int fd);
	static void removePollfd(int fd);

	static bool isPollSet(int fd);
	static bool isPollOutSet(int fd);
	static bool isPollInSet(int fd);
	static void setPollOut(int fd, bool enabled);
	static void addServer(const ServerConfig& config);

private:
	static pollfd* findPollfd(int fd);

	static std::vector<Webserv*>	_servers;
	static std::vector<pollfd>		_fds;

};
