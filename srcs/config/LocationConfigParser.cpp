#include <config/LocationConfigParser.hpp>

namespace Webserver
{
	LocationConfigParser::LocationConfigParser(StreamData* streamData, const std::string& pattern, const HostFields& parentData) :
		AParseTreeLeaf::AParseTreeLeaf(streamData),
		_parentData(&parentData)
	{
		_data->_pattern = pattern;
	}

	LocationConfigParser::LocationConfigParser(StreamData* streamData) : AParseTreeLeaf::AParseTreeLeaf(streamData)
	{
	}

	void LocationConfigParser::fillData()
	{
		*_data = *_parentData;
		AParseTreeLeaf<LocationConfig>::fillData();
	}

	std::map<std::string, ICommand*> LocationConfigParser::createKeywords()
	{
		std::map<std::string, ICommand*> keywords;

		keywords["autoindex"]			= new ParseVariableCommand<bool>(&_data->_autoIndexEnabled);
		keywords["root"]				= new ParseVariableCommand<std::string>(&_data->_root);
		keywords["default_index"]		= new ParseVariableCommand<std::string>(&_data->_defaultIndex);
		keywords["default_error"]		= new ParseVariableCommand<std::string>(&_data->_defaultError);
		keywords["accepted_methods"]	= new ParseVariableCommand<std::vector<Method::method> >(&_data->_acceptedMethods);
		keywords["allow_upload"]		= new ParseVariableCommand<bool>(&_data->_allowUpload);
		keywords["error_page"]			= new AppendVariableCommand<HostFields::ErrorPages>(&_data->_errorPages);

		keywords["route_type"]	= new ParseVariableCommand<RouteType::RouteType>(&_data->_routeType);

		return keywords;
	}

	void LocationConfigParser::validate()
	{
	}
}
