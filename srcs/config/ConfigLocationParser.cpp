#include <config/ConfigLocationParser.hpp>

namespace Webserver
{
	ConfigLocationParser::ConfigLocationParser(StreamData streamData) : AConfigParser::AConfigParser(streamData, initKeywords())
	{
	}

	std::map<std::string, AConfigParser::ICommand*> ConfigLocationParser::initKeywords()
	{
		std::map<std::string, ICommand*> keywords;
		keywords["route"]		= new ParseVariableCommand<std::string>(&_data._route);
		return keywords;
	}
}
