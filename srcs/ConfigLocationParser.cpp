#include <ConfigLocationParser.hpp>

namespace ConfigFileParser
{
	ConfigLocationParser::ConfigLocationParser(StreamData streamData) : AConfigParser::AConfigParser(streamData, initKeywords())
	{
	}

	std::map<std::string, AConfigParser::ICommand*> ConfigLocationParser::initKeywords()
	{
		std::map<std::string, ICommand*> keywords;
		keywords["route"]		= new ParseVariableCommand<std::string>(&_route);
		return keywords;
	}
}
