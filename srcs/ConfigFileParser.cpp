#include <ConfigFileParser.hpp>

ConfigFileParser::ConfigFileParser(const std::string& filePath) : _filePath(filePath)
{
}

ConfigFileParser::~ConfigFileParser()
{
}

/*
	On success, returns a vector containing all parsed server configurations.
	If the parser encounters an error, this function will not return and throw an exception instead.
*/
std::vector<ServerConfig> ConfigFileParser::parse()
{
}
