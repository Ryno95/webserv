#pragma once

#include <vector>

#include <HostConfig.hpp>

typedef unsigned int uint;

class ServerConfig
{
public:
	ServerConfig();
	~ServerConfig();

private:
	uint _port;
	uint _backlog;
	std::vector<const HostConfig> _hosts;

};
