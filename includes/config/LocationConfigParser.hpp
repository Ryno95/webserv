#pragma once

#include <config/AParseTreeLeaf.hpp>
// #include <config/HostConfigParser.hpp>
#include <config/LocationConfig.hpp>

namespace Webserver
{
	class LocationConfigParser : public AParseTreeLeaf<LocationConfig>
	{
	public:
		LocationConfigParser(StreamData* streamData, const std::map<std::string, ICommand*>& hostKeywords, const std::string& pattern);
		LocationConfigParser(StreamData* streamData);

		void validate();
		std::map<std::string, ICommand*> createKeywords();

	private:
		std::map<std::string, ICommand*> _hostKeywords;
	};
}
