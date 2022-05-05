#pragma once

#include <vector>

#include <config/HostConfig.hpp>

typedef unsigned int uint;

struct ServerConfig
{
	ServerConfig& operator=(const ServerConfig& ref)
	{
		(void)ref;
		return *this;
	}

	uint							port;
	uint							buffer_size;
	uint							listen_backlog;
	std::string						mimeTypesFile;
	std::vector<const HostConfig>	hosts;
};
