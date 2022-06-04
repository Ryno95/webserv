#include <ConfigServerParser.hpp>
#include <ConfigHostParser.hpp>

namespace ConfigFileParser
{
	ConfigServerParser::ConfigServerParser(StreamData streamData) : AConfigParser::AConfigParser(streamData, initKeywords())
	{
	}

	std::map<std::string, AConfigParser::ICommand*> ConfigServerParser::initKeywords()
	{
		std::map<std::string, ICommand*> keywords;
		keywords["port"]		= new ParseVariableCommand<uint>(&_port);
		keywords["host"]		= new CreateChildCommand<ConfigHostParser>(this);
		return keywords;
	}
}
