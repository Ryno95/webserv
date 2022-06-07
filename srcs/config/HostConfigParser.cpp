#include <config/HostConfigParser.hpp>
#include <config/LocationConfigParser.hpp>

namespace Webserver
{
	HostConfigParser::HostConfigParser(StreamData streamData) : AConfigParser::AConfigParser(streamData, initKeywords())
	{
	}

	std::map<std::string, AConfigParser::ICommand*> HostConfigParser::initKeywords()
	{
		std::map<std::string, ICommand*> keywords;
		keywords["location"]	= new CreateChildCommand<LocationConfigParser>(this);

		keywords["name"]		= new ParseVariableCommand<std::vector<std::string> >(&_data._names);

		keywords["name"]		= new ParseVariableCommand<bool>(&_data._autoIndexEnabled);
		keywords["name"]		= new ParseVariableCommand<std::string>(&_data._root);
		keywords["name"]		= new ParseVariableCommand<std::string>(&_data._defaultIndex);
		keywords["name"]		= new ParseVariableCommand<std::string>(&_data._defaultError);
		keywords["name"]		= new ParseVariableCommand<std::vector<Method::method> >(&_data._acceptedMethods);
		return keywords;
	}
}
