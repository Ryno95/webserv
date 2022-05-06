#pragma once

#include <vector>

#include <HostConfig.hpp>

typedef unsigned int uint;

struct ServerConfig
{
	ServerConfig() {}

	ServerConfig(const ServerConfig& rhs)
	{
		*this = rhs;
	}

	ServerConfig& operator=(const ServerConfig& ref)
	{
		port = ref.port;
		buffer_size = ref.buffer_size;
		listen_backlog = ref.listen_backlog;
		mimeTypesFile = ref.mimeTypesFile;
		hosts = ref.hosts;
		return *this;
	}

	uint							port;
	uint							buffer_size;
	uint							listen_backlog;
	std::string						mimeTypesFile;
	std::vector<HostConfig>	hosts;
};
