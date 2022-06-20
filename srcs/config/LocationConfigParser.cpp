#include <config/LocationConfigParser.hpp>

namespace Webserver
{
	LocationConfigParser::LocationConfigParser(StreamData* streamData, const std::string& pattern) :
		AParseTreeLeaf::AParseTreeLeaf(streamData)
	{
		_data->_pattern = pattern;
	}

	LocationConfigParser::LocationConfigParser(StreamData* streamData) : AParseTreeLeaf::AParseTreeLeaf(streamData)
	{
	}

	std::map<std::string, ICommand*> LocationConfigParser::createKeywords()
	{
		std::map<std::string, ICommand*> keywords;

		keywords["autoindex"]			= new ParseVariableCommand<bool>(&_data->_autoIndexEnabled);
		keywords["root"]				= new ParseVariableCommand<std::string>(&_data->_root);
		keywords["default_index"]		= new ParseVariableCommand<std::string>(&_data->_defaultIndex);
		keywords["default_error"]		= new ParseVariableCommand<std::string>(&_data->_defaultError);
		keywords["accepted_methods"]	= new ParseVariableCommand<std::vector<Method::method> >(&_data->_acceptedMethods);

		keywords["route_type"]	= new ParseVariableCommand<RouteType::RouteType>(&_data->_routeType);

		return keywords;
	}

	void LocationConfigParser::validate()
	{
	}
}
