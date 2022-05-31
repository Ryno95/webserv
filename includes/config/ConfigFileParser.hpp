#pragma once

#include <stdexcept>
#include <string>
#include <map>
#include <vector>
#include <iostream>

#include <config/ServerConfig.hpp>
#include <config/GlobalConfig.hpp>
#include <config/LocationConfig.hpp>
#include <Logger.hpp>
#include <Utility.hpp>
#include <Exception.hpp>

namespace Webserver
{
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
			IN_APPLICATION_BLOCK,

			PRE_LOCATION_BLOCK,
			IN_LOCATION_BLOCK
		} state;

		template<class T>
		void parseVariableIntoConfig(const std::string& line, T& config) const;

		void validateAllConfigs() const;
	

		std::vector<ServerConfig> _serverConfigs;
		std::string _filePath;
		size_t _lineCount;
		std::map<std::string, int> _hostVariables;
		std::map<std::string, int> _serverVariables;

	};

	template<class T>
	void ConfigFileParser::parseVariableIntoConfig(const std::string& line, T& config) const
	{
		size_t pos = std::min(line.find('\t'), line.find(' '));
		if (pos == std::string::npos)
			throw ConfigParseException(_lineCount, line);

		std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 1, line.size() - pos);
		value = removeLeadingWhitespace(value);

		config.parseVariable(key, value);
	}
}
