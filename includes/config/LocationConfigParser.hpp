#pragma once

#include <config/HostConfigParser.hpp>
#include <config/LocationConfig.hpp>

namespace Webserver
{
	class LocationConfigParser : public HostConfigParser
	{
	public:
		LocationConfigParser(StreamData* streamData, const std::string& pattern);

	private:
		std::map<std::string, ICommand*> createKeywords();
		void validate();

		LocationConfig _data;
	};
}
