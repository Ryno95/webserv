#include <ConfigHostParser.hpp>
#include <ConfigLocationParser.hpp>

namespace ConfigFileParser
{
	ConfigHostParser::ConfigHostParser(StreamData streamData) : AConfigParser::AConfigParser(streamData, initKeywords())
	{
	}

	std::map<std::string, AConfigParser::ICommand*> ConfigHostParser::initKeywords()
	{
		std::map<std::string, ICommand*> keywords;
		keywords["name"]		= new ParseVariableCommand<std::vector<std::string>>(&_names);
		keywords["location"]	= new CreateChildCommand<ConfigLocationParser>(this);
		return keywords;
	}
}
