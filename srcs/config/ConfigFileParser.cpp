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

	void ConfigFileParser::validateAllConfigs() const
	{
		GlobalConfig::get().validate();
		
		for (size_t i = 0; i < _serverConfigs.size(); ++i)
		{
			_serverConfigs[i].validate();
			for (size_t j = 0; j < _serverConfigs[i].hosts.size(); j++)
			{
				_serverConfigs[i].hosts[j].validate();
				for (size_t k = 0; k < _serverConfigs[i].hosts[j].locations.size(); k++)
				{
					_serverConfigs[i].hosts[j].locations[k].validate();
				}
			}
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
		LocationConfig currentLocationConfig;

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
					parseVariableIntoConfig(line, currentServerConfig);
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
				else if (line.find("location") != std::string::npos)
				{
					DEBUG("GOING IN LOCATION!");
					parseVariableIntoConfig(line, currentLocationConfig);
					currentState = PRE_LOCATION_BLOCK;
				}
				else
					parseVariableIntoConfig(line, currentHostConfig);
				break;

			case PRE_LOCATION_BLOCK:
				if (line == "{")
					currentState = IN_LOCATION_BLOCK;
				else
					throw ConfigParseUnexpectedTokenException("{", _lineCount, line);
				break;

			case IN_LOCATION_BLOCK:
				if (line == "}")
				{
					currentHostConfig.locations.push_back(currentLocationConfig);
					currentLocationConfig = LocationConfig();
					currentState = IN_HOST_BLOCK;
				}
				else
					parseVariableIntoConfig(line, currentLocationConfig);
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
					parseVariableIntoConfig(line, currentGlobalConfig);
				break;
			}
		}

		if (currentState != NONE)
			throw ConfigParseUnexpectedTokenException("}", _lineCount, line);

		validateAllConfigs();

		GlobalConfig::set(currentGlobalConfig);
		return _serverConfigs;
	}
}
