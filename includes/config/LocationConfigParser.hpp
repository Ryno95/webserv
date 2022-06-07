#pragma once

#include <config/AConfigParser.hpp>
#include <config/LocationConfig.hpp>

namespace Webserver
{
	class LocationConfigParser : public AConfigParser
	{
	public:
		LocationConfigParser(StreamData* streamData);

	protected:
		std::map<std::string, ICommand*> initKeywords();

	private:
		Webserver::LocationConfig _data;
	};
}
