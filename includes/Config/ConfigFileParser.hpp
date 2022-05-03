#pragma once

#include <string>
#include <vector>

#include <ServerConfig.hpp>

class ConfigFileParser
{
public:
	ConfigFileParser(const std::string& filePath);
	~ConfigFileParser();

	std::vector<ServerConfig> parse();

private:
	HostConfig parseHostConfig() const;
	ServerConfig parseServerConfig() const;

	std::vector<ServerConfig> _serverConfigs;
	std::string _filePath;

};
