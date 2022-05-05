#pragma once

#include <string>
#include <map>
#include <vector>
#include <iostream>

#include <ServerConfig.hpp>
#include <Logger.hpp>

namespace ServerVariables
{
// Still needs to be implemented in config file:
	// #define MAX_TARGET_LEN 50
	// #define TIMEOUT_MS 1000
	enum vars
	{
		LISTEN_PORT,
		BUFFER_SIZE,
		MIME_CONFIG,
		LISTEN_BACKLOG
	};
}

namespace HostVariables
{
	enum vars
	{
		NAME,
		ROOT,
		METHODS,
		DEFAULT_INDEX,
		DEFAULT_ERROR
	};
}

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
		IN_HOST_BLOCK
	} state;

	template<class T>
	void parseVariable(const std::string& value, T* dest) const;

	void parseHostVariable(const std::string& line, HostConfig& config) const;
	void parseServerVariable(const std::string& line, ServerConfig& config) const;

	void fillServerVariablesMap();
	void fillHostVariablesMap();


	std::vector<ServerConfig> _serverConfigs;
	std::string _filePath;
	size_t _lineCount;
	std::map<std::string, int> _hostVariables;
	std::map<std::string, int> _serverVariables;

};

template<>
inline void ConfigFileParser::parseVariable(const std::string& value, uint* dest) const
{
	*dest = atoi(value.c_str());
}

template<>
inline void ConfigFileParser::parseVariable(const std::string& value, std::string* dest) const
{
	*dest = value;
}

std::string removeLeadingWhitespace(const std::string &str); // prototype temporary, move to utils class

template<>
inline void ConfigFileParser::parseVariable(const std::string& value, std::vector<std::string>* dest) const
{
	std::string str = value;

	while (true)
	{
		size_t end = std::min(std::min(str.find(' '), str.find('\t')), str.size());
		dest->push_back(str.substr(0, end));

		if (end == str.size())
			return;

		str = str.substr(end, str.size() - end);
		str = removeLeadingWhitespace(str);
	}
}

template<>
inline void ConfigFileParser::parseVariable(const std::string& value, std::vector<Method::method>* dest) const
{
	std::string str = value;
	std::string method;

	while (true)
	{
		size_t end = std::min(std::min(str.find(' '), str.find('\t')), str.size());
		method = str.substr(0, end);

		if (method == "GET")
			dest->push_back(Method::GET);
		else if (method == "POST")
			dest->push_back(Method::POST);
		else if (method == "DELETE")
			dest->push_back(Method::DELETE);
		else
		{
			ERROR("Unhandled method supplied: " << method);
			throw std::runtime_error("Unhandled method supplied");
		}

		if (end == str.size())
			return;

		str = str.substr(end, str.size() - end);
		str = removeLeadingWhitespace(str);
	}
}
