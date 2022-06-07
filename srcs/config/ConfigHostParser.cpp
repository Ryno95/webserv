#include <config/ConfigHostParser.hpp>
#include <config/ConfigLocationParser.hpp>

namespace Webserver
{
	ConfigHostParser::ConfigHostParser(StreamData streamData) : AConfigParser::AConfigParser(streamData, initKeywords())
	{
	}

	std::map<std::string, AConfigParser::ICommand*> ConfigHostParser::initKeywords()
	{
		std::map<std::string, ICommand*> keywords;
		keywords["location"]	= new CreateChildCommand<ConfigLocationParser>(this);

		keywords["name"]		= new ParseVariableCommand<std::vector<std::string>>(&_data._names);

		keywords["name"]		= new ParseVariableCommand<bool>(&_data._autoIndexEnabled);
		keywords["name"]		= new ParseVariableCommand<std::string>(&_data._root);
		keywords["name"]		= new ParseVariableCommand<std::string>(&_data._defaultIndex);
		keywords["name"]		= new ParseVariableCommand<std::string>(&_data._defaultError);
		keywords["name"]		= new ParseVariableCommand<std::vector<Method::method>>(&_data._acceptedMethods);
		return keywords;
	}
}
