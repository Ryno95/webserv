#pragma once

#include <vector>

#include <config/HostConfig.hpp>

typedef unsigned int uint;

struct ServerConfig
{
	uint							port;
	uint							buffer_size;
	uint							listen_backlog;
	std::string						mimeTypesFile;
	std::vector<const HostConfig>	hosts;
};
