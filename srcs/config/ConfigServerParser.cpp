#include <config/ConfigServerParser.hpp>
#include <config/ConfigHostParser.hpp>

namespace Webserver
{
	ConfigServerParser::ConfigServerParser(StreamData streamData) : AConfigParser::AConfigParser(streamData, initKeywords())
	{
	}

	std::map<std::string, AConfigParser::ICommand*> ConfigServerParser::initKeywords()
	{
		std::map<std::string, ICommand*> keywords;
		keywords["host"]		= new CreateChildCommand<ConfigHostParser>(this);

		keywords["port"]		= new ParseVariableCommand<uint>(&_data._port);
		return keywords;
	}
}
