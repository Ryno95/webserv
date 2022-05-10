#pragma once

#include <stdexcept>
#include <string>
#include <map>
#include <vector>
#include <iostream>

#include <config/ServerConfig.hpp>
#include <config/GlobalConfig.hpp>
#include <Logger.hpp>
#include <Utility.hpp>
#include <Exception.hpp>

class ConfigFileParser
{
public:
	ConfigFileParser(const std::string& filePath);
	~ConfigFileParser();

	std::vector<ServerConfig>& parse();

private:
	typedef enum
	{
		NONE,

		PRE_SERVER_BLOCK,
		IN_SERVER_BLOCK,

		PRE_HOST_BLOCK,
		IN_HOST_BLOCK,

		PRE_APPLICATION_BLOCK,
		IN_APPLICATION_BLOCK
	} state;

	template<class T>
	void parseVariable(const std::string& value, T* dest) const;

	void parseGlobalVariable(const std::string& line, GlobalConfig& config) const;
	void parseHostVariable(const std::string& line, HostConfig& config) const;
	void parseServerVariable(const std::string& line, ServerConfig& config) const;

	std::vector<ServerConfig> _serverConfigs;
	std::string _filePath;
	size_t _lineCount;
	std::map<std::string, int> _hostVariables;
	std::map<std::string, int> _serverVariables;

};
