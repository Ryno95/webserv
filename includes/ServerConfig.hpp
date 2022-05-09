#pragma once

#include <vector>

#include <HostConfig.hpp>
#include <AConfig.hpp>

typedef unsigned int uint;

struct ServerConfig : public AConfig
{
	ServerConfig();
	ServerConfig(const ServerConfig& ref);
	~ServerConfig();
	ServerConfig& operator=(const ServerConfig& ref);

	uint					port;
	uint					bufferSize;
	uint					listenBacklog;
	std::string				mimeTypesFile;
	std::vector<HostConfig>	hosts;

private:
	map_type fillVariablesMap();

};
