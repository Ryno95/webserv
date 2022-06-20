#pragma once

#include <config/AParseTreeLeaf.hpp>
#include <config/LocationConfig.hpp>

namespace Webserver
{
	class LocationConfigParser : public AParseTreeLeaf<LocationConfig>
	{
	public:
		LocationConfigParser(StreamData* streamData, const std::string& pattern);
		LocationConfigParser(StreamData* streamData);

		void validate();
		std::map<std::string, ICommand*> createKeywords();
	};
}
