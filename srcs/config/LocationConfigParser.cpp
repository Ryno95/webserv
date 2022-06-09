#include <config/LocationConfigParser.hpp>

namespace Webserver
{
	LocationConfigParser::LocationConfigParser(StreamData* streamData, const std::map<std::string, ICommand*>& hostKeywords, const std::string& pattern) :
		AParseTreeLeaf::AParseTreeLeaf(streamData), _hostKeywords(hostKeywords)
	{
		_data->_pattern = pattern;
	}

	LocationConfigParser::LocationConfigParser(StreamData* streamData) : AParseTreeLeaf::AParseTreeLeaf(streamData)
	{
	}


	std::map<std::string, ICommand*> LocationConfigParser::createKeywords()
	{
		std::map<std::string, ICommand*> keywords = _hostKeywords;

		keywords["route_type"]	= new ParseVariableCommand<RouteType::RouteType>(&_data->_routeType);

		return keywords;
	}

	void LocationConfigParser::validate()
	{
	}
}
