#pragma once

#include <string>
#include <vector>

#include <HostConfig.hpp>

typedef unsigned int uint;

class ServerConfig
{
public:
	ServerConfig(const std::string& configFilePath);
	~ServerConfig();

private:
	uint _port;
	uint _backlog;
	std::vector<HostConfig> _hosts;

};
