#include <config/LocationConfigParser.hpp>

namespace Webserver
{
	LocationConfigParser::LocationConfigParser(StreamData* streamData, const std::string& pattern) : HostConfigParser::HostConfigParser(streamData)
	{
		_data._pattern = pattern;
	}

	std::map<std::string, AConfigParser::ICommand*> LocationConfigParser::createKeywords()
	{
		std::map<std::string, ICommand*> keywords;

		addProtectedKeywords(keywords);
		return keywords;
	}

	void LocationConfigParser::validate()
	{
		HostConfigParser::validate();
	}
}
