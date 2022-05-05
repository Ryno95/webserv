#include <fstream>
#include <sstream>
#include <map>

#include <ConfigFileParser.hpp>
#include <Logger.hpp>

std::string removeLeadingWhitespace(const std::string &str)
{
	std::string out = str;
	std::string::const_iterator iter = str.begin();
	std::string::const_iterator end = str.end();
	size_t count = 0;

	while (iter != end)
	{
		if (*iter == ' ' || *iter == '\t')
			count++;
		else
			break;
		iter++;
	}

	out.erase(0, count);
	return out;
}

std::string removeTrailingWhitespace(const std::string &str)
{
	std::string out = str;
	std::string::const_reverse_iterator iter = str.rbegin();
	std::string::const_reverse_iterator end = str.rend();
	size_t count = 0;

	while (iter != end)
	{
		if (*iter == ' ' || *iter == '\t')
			count++;
		else
			break;
		iter++;
	}

	out.erase(out.size() - count, count);
	return out;
}

ConfigFileParser::ConfigFileParser(const std::string &filePath) : _filePath(filePath), _lineCount(0)
{
	fillHostVariablesMap();
	fillServerVariablesMap();
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
	std::string line;
	state currentState = NONE;
	ServerConfig currentServerConfig;
	HostConfig currentHostConfig;

	if (fstream.fail())
		throw std::runtime_error("Error opening config file");

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
		}
	}

	if (currentState != NONE)
	{
		ERROR("Unclosed section encountered");
		throw std::runtime_error("Parse error");
	}
	return _serverConfigs;
}

void ConfigFileParser::fillHostVariablesMap()
{
	_hostVariables["name"] = HostVariables::NAME;
	_hostVariables["root"] = HostVariables::ROOT;
	_hostVariables["methods"] = HostVariables::METHODS;
	_hostVariables["default_index"] = HostVariables::DEFAULT_INDEX;
	_hostVariables["default_error"] = HostVariables::DEFAULT_ERROR;
}

void ConfigFileParser::fillServerVariablesMap()
{
	_serverVariables["listen_port"] = ServerVariables::LISTEN_PORT;
	_serverVariables["buffer_size"] = ServerVariables::BUFFER_SIZE;
	_serverVariables["mime_config"] = ServerVariables::MIME_CONFIG;
	_serverVariables["listen_backlog"] = ServerVariables::LISTEN_BACKLOG;
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
	value = removeLeadingWhitespace(value);

	std::map<std::string, int>::const_iterator result = _serverVariables.find(key);
	if (result == _serverVariables.end())
	{
		ERROR("Invalid server-variable encountered on line [" << _lineCount << "]: " << line);
		throw std::runtime_error("Invalid variable encountered");
	}

	switch (result->second)
	{
	case ServerVariables::LISTEN_PORT:
		parseVariable(value, &config.port);
		break;
	case ServerVariables::BUFFER_SIZE:
		parseVariable(value, &config.buffer_size);
		break;
	case ServerVariables::MIME_CONFIG:
		parseVariable(value, &config.mimeTypesFile);
		break;
	case ServerVariables::LISTEN_BACKLOG:
		parseVariable(value, &config.listen_backlog);
		break;
	}
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
	value = removeLeadingWhitespace(value);

	std::map<std::string, int>::const_iterator result = _hostVariables.find(key);
	if (result == _hostVariables.end())
	{
		ERROR("Invalid host-variable encountered on line [" << _lineCount << "]: " << line);
		throw std::runtime_error("Invalid variable encountered");
	}

	switch (result->second)
	{
	case HostVariables::NAME:
		parseVariable(value, &config.names);
		break;
	case HostVariables::ROOT:
		parseVariable(value, &config.root);
		break;
	case HostVariables::METHODS:
		parseVariable(value, &config.acceptedMethods);
		break;
	case HostVariables::DEFAULT_INDEX:
		parseVariable(value, &config.defaultIndex);
		break;
	case HostVariables::DEFAULT_ERROR:
		parseVariable(value, &config.defaultError);
		break;
	}
}
