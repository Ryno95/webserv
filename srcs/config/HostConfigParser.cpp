#include <config/HostConfigParser.hpp>
#include <config/LocationConfigParser.hpp>

namespace Webserver
{
	HostConfigParser::HostConfigParser(StreamData* streamData) : AConfigParser::AConfigParser(streamData)
	{
	}

	void HostConfigParser::addProtectedKeywords(std::map<std::string, ICommand*>& keywords)
	{
		keywords["autoindex"]			= new ParseVariableCommand<bool>(&_data._autoIndexEnabled);
		keywords["root"]				= new ParseVariableCommand<std::string>(&_data._root);
		keywords["default_index"]		= new ParseVariableCommand<std::string>(&_data._defaultIndex);
		keywords["default_error"]		= new ParseVariableCommand<std::string>(&_data._defaultError);
		keywords["accepted_methods"]	= new ParseVariableCommand<std::vector<Method::method> >(&_data._acceptedMethods);
	}

	std::map<std::string, AConfigParser::ICommand*> HostConfigParser::createKeywords()
	{
		std::map<std::string, ICommand*> keywords;
		keywords["location"]	= new CreateChildCommand<LocationConfigParser>(this);

		keywords["name"]		= new ParseVariableCommand<std::vector<std::string> >(&_data._names);

		addProtectedKeywords(keywords);
		return keywords;
	}
}
