#include <fstream>
#include <sstream>
#include <map>

#include <ConfigFileParser.hpp>
#include <Logger.hpp>

ConfigFileParser::ConfigFileParser(const std::string &filePath) : _filePath(filePath), _lineCount(0)
{
}

ConfigFileParser::~ConfigFileParser()
{
}

/*
	On success, returns a vector containing all parsed server configurations.
	If the parser encounters an error, this function will not return and throw an exception instead.
*/
std::vector<ServerConfig>& ConfigFileParser::parse()
{
	std::ifstream fstream(_filePath);
	if (fstream.fail())
		throw std::runtime_error("Error opening config file");

	std::string line;
	state currentState = NONE;
	ServerConfig currentServerConfig;
	HostConfig currentHostConfig;
	GlobalConfig currentGlobalConfig;

	while (std::getline(fstream, line))
	{
		_lineCount++;
		line = Util::removeLeadingWhitespace(line);
		line = Util::removeTrailingWhitespace(line);

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
			{
				ERROR("Invalid input on line [" << _lineCount << "]: " << line);
				throw std::runtime_error("Parse error");
			}
			break;

		case PRE_SERVER_BLOCK:
			if (line == "{")
				currentState = IN_SERVER_BLOCK;
			else
			{
				ERROR("Expected '{' on line [" << _lineCount << "]: " << line);
				throw std::runtime_error("Parse error");
			}
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
			{
				ERROR("Expected '{' on line [" << _lineCount << "]: " << line);
				throw std::runtime_error("Parse error");
			}
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
			{
				ERROR("Expected '{' on line [" << _lineCount << "]: " << line);
				throw std::runtime_error("Parse error");
			}
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
	{
		ERROR("Unclosed section encountered");
		throw std::runtime_error("Parse error");
	}

	GlobalConfig::set(currentGlobalConfig);
	return _serverConfigs;
}

void ConfigFileParser::parseGlobalVariable(const std::string &line, GlobalConfig& config) const
{
	size_t pos = std::min(line.find('\t'), line.find(' '));
	if (pos == std::string::npos)
	{
		ERROR("No value assigned to variable on line [" << _lineCount << "]: " << line);
		throw std::runtime_error("No value for variable");
	}
	std::string key = line.substr(0, pos);
	std::string value = line.substr(pos + 1, line.size() - pos);
	value = Util::removeLeadingWhitespace(value);

	config.parseVariable(key, value);
}

void ConfigFileParser::parseServerVariable(const std::string &line, ServerConfig &config) const
{
	size_t pos = std::min(line.find('\t'), line.find(' '));
	if (pos == std::string::npos)
	{
		ERROR("No value assigned to variable on line [" << _lineCount << "]: " << line);
		throw std::runtime_error("No value for variable");
	}
	std::string key = line.substr(0, pos);
	std::string value = line.substr(pos + 1, line.size() - pos);
	value = Util::removeLeadingWhitespace(value);

	config.parseVariable(key, value);
}

void ConfigFileParser::parseHostVariable(const std::string &line, HostConfig &config) const
{
	size_t pos = std::min(line.find('\t'), line.find(' '));
	if (pos == std::string::npos)
	{
		ERROR("No value assigned to variable on line [" << _lineCount << "]: " << line);
		throw std::runtime_error("No value for variable");
	}
	std::string key = line.substr(0, pos);
	std::string value = line.substr(pos + 1, line.size() - pos);
	value = Util::removeLeadingWhitespace(value);

	config.parseVariable(key, value);
}
