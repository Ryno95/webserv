#include <config/HostConfigParser.hpp>
#include <config/LocationConfigParser.hpp>

namespace Webserver
{
	void HostConfigParser::addChild(const std::string& args)
	{
		if (args.size() == 0)
			throw std::runtime_error("No pattern assigned to Location");
		
		_children.push_back(new LocationConfigParser(_streamData, args, *_data));
		_children.back()->useBrackets();
		if (_children.back()->readStream() == false)
			throw std::runtime_error("Unclosed section encountered.");
	}

	HostConfigParser::HostConfigParser(StreamData* streamData) : AParseTreeBranch::AParseTreeBranch(streamData)
	{
	}

	std::map<std::string, ICommand*> HostConfigParser::createKeywords()
	{
		std::map<std::string, ICommand*> keywords;

		keywords["autoindex"]			= new ParseVariableCommand<bool>(&_data->_autoIndexEnabled);
		keywords["root"]				= new ParseVariableCommand<std::string>(&_data->_root);
		keywords["default_index"]		= new ParseVariableCommand<std::string>(&_data->_defaultIndex);
		keywords["default_error"]		= new ParseVariableCommand<std::string>(&_data->_defaultError);
		keywords["accepted_methods"]	= new ParseVariableCommand<std::vector<Method::method> >(&_data->_acceptedMethods);

		keywords["location"]	= new CreateChildCommand<LocationConfigParser>(this);

		keywords["name"]		= new ParseVariableCommand<std::vector<std::string> >(&_data->_names);

		return keywords;
	}

	void HostConfigParser::validate()
	{
	}
}
