#include <string>
#include <fstream>
#include <sstream>
#include <map>

#include <Utility.hpp>
#include <config/ConfigFileParser.hpp>
#include <Logger.hpp>

namespace Webserver
{
	ConfigFileParser::ConfigFileParser(const std::string &filePath) : _filePath(filePath), _lineCount(0)
	{
	}

	ConfigFileParser::~ConfigFileParser()
	{
	}

	void ConfigFileParser::validateConfigs()
	{
		GlobalConfig::get().validateGlobalConfig();
		
		for (size_t i = 0; i < _serverConfigs.size(); ++i)
		{
			_serverConfigs[i].validateServerConfig();
			for (size_t j = 0; j < _serverConfigs[i].hosts.size(); j++)
				_serverConfigs[i].hosts[j].validateHostConfig();
		}	
	}

	/*
		On success, returns a vector containing all parsed server configurations.
		If the parser encounters an error, this function will not return and throw an exception instead.
	*/
	std::vector<ServerConfig>& ConfigFileParser::parse()
	{
		std::ifstream fstream(_filePath);
		if (fstream.fail())
			throw FileNotFoundException(_filePath);

		std::string line;
		state currentState = NONE;
		ServerConfig currentServerConfig;
		HostConfig currentHostConfig;
		GlobalConfig currentGlobalConfig;

		while (std::getline(fstream, line))
		{
			_lineCount++;
			line = removeLeadingWhitespace(line);
			line = removeTrailingWhitespace(line);

			if (line.size() == 0 || line.front() == '#')
				continue;

			switch (currentState)
			{
			case NONE:
				if (line == "server")
					currentState = PRE_SERVER_BLOCK;
				else if (line == "application")
					currentState = PRE_APPLICATION_BLOCK;
				else
					throw ConfigParseUnexpectedTokenException("[server] or [application]", _lineCount, line);
				break;

			case PRE_SERVER_BLOCK:
				if (line == "{")
					currentState = IN_SERVER_BLOCK;
				else
					throw ConfigParseUnexpectedTokenException("{", _lineCount, line);
				break;

			case IN_SERVER_BLOCK:
				if (line == "}")
				{
					_serverConfigs.push_back(currentServerConfig);
					currentServerConfig = ServerConfig();
					currentState = NONE;
				}
				else if (line == "host")
					currentState = PRE_HOST_BLOCK;
				else
					parseServerVariable(line, currentServerConfig);
				break;

			case PRE_HOST_BLOCK:
				if (line == "{")
					currentState = IN_HOST_BLOCK;
				else
					throw ConfigParseUnexpectedTokenException("{", _lineCount, line);
				break;

			case IN_HOST_BLOCK:
				if (line == "}")
				{
					currentServerConfig.hosts.push_back(currentHostConfig);
					currentHostConfig = HostConfig();
					currentState = IN_SERVER_BLOCK;
				}
				else
					parseHostVariable(line, currentHostConfig);
				break;
			
			case PRE_APPLICATION_BLOCK:
				if (line == "{")
					currentState = IN_APPLICATION_BLOCK;
				else
					throw ConfigParseUnexpectedTokenException("{", _lineCount, line);
				break;

			case IN_APPLICATION_BLOCK:
				if (line == "}")
					currentState = NONE;
				else
					parseGlobalVariable(line, currentGlobalConfig);
				break;
			}
		}

		if (currentState != NONE)
			throw ConfigParseUnexpectedTokenException("}", _lineCount, line);

		GlobalConfig::set(currentGlobalConfig);
		// void validateConfigs(std::vector<ServerConfig>	&configs)
		validateConfigs();
		return _serverConfigs;
	}

	void ConfigFileParser::parseGlobalVariable(const std::string &line, GlobalConfig& config) const
	{
		size_t pos = std::min(line.find('\t'), line.find(' '));
		if (pos == std::string::npos)
			throw ConfigParseException(_lineCount, line);

		std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 1, line.size() - pos);
		value = removeLeadingWhitespace(value);

		config.parseVariable(key, value);
	}

	void ConfigFileParser::parseServerVariable(const std::string &line, ServerConfig &config) const
	{
		size_t pos = std::min(line.find('\t'), line.find(' '));
		if (pos == std::string::npos)
			throw ConfigParseException(_lineCount, line);

		std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 1, line.size() - pos);
		value = removeLeadingWhitespace(value);

		config.parseVariable(key, value);
	}

	void ConfigFileParser::parseHostVariable(const std::string &line, HostConfig &config) const
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
