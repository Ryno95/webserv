#include <config/HostConfigParser.hpp>
#include <config/LocationConfigParser.hpp>

namespace Webserver
{
	void HostConfigParser::addChild(const std::string& args)
	{
		if (args.size() == 0)
			throw std::runtime_error("No pattern assigned to Location");
		
		std::map<std::string, ICommand*> keywords;
		addProtectedKeywords(keywords);
		_children.push_back(new LocationConfigParser(_streamData, keywords, args));
		_children.back()->useBrackets();
		if (_children.back()->readStream() == false)
			throw std::runtime_error("Unclosed section encountered.");
		_children.back()->validate();
		_data->_children.push_back(_children.back()->getData());
	}

	HostConfigParser::HostConfigParser(StreamData* streamData) : AParseTreeBranch::AParseTreeBranch(streamData)
	{
	}

	void HostConfigParser::addProtectedKeywords(std::map<std::string, ICommand*>& keywords)
	{
		keywords["autoindex"]			= new ParseVariableCommand<bool>(&_data->_autoIndexEnabled);
		keywords["root"]				= new ParseVariableCommand<std::string>(&_data->_root);
		keywords["default_index"]		= new ParseVariableCommand<std::string>(&_data->_defaultIndex);
		keywords["default_error"]		= new ParseVariableCommand<std::string>(&_data->_defaultError);
		keywords["accepted_methods"]	= new ParseVariableCommand<std::vector<Method::method> >(&_data->_acceptedMethods);
	}

	std::map<std::string, ICommand*> HostConfigParser::createKeywords()
	{
		std::map<std::string, ICommand*> keywords;
		keywords["location"]	= new CreateChildCommand<LocationConfigParser>(this);

		keywords["name"]		= new ParseVariableCommand<std::vector<std::string> >(&_data->_names);

		addProtectedKeywords(keywords);
		return keywords;
	}

	void HostConfigParser::validate()
	{
		if (_data->_acceptedMethods.size() == 0)
		{
			WARN("One of the configured hosts does not accept any HTTP method.");
		}
	}
}
