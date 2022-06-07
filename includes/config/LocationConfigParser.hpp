#pragma once

#include <config/HostConfigParser.hpp>
#include <config/LocationConfig.hpp>

namespace Webserver
{
	class LocationConfigParser : public HostConfigParser
	{
	public:
		LocationConfigParser(StreamData* streamData);

	private:
		std::map<std::string, ICommand*> createKeywords();

		Webserver::LocationConfig _data;
	};
}
