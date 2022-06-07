#include <config/LocationConfigParser.hpp>

namespace Webserver
{
	LocationConfigParser::LocationConfigParser(StreamData* streamData) : HostConfigParser::HostConfigParser(streamData)
	{
	}

	std::map<std::string, AConfigParser::ICommand*> LocationConfigParser::createKeywords()
	{
		std::map<std::string, ICommand*> keywords;
		keywords["route"]		= new ParseVariableCommand<std::string>(&_data._route);

		addProtectedKeywords(keywords);
		return keywords;
	}
}
