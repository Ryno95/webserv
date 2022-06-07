#pragma once

#include <config/AConfigParser.hpp>
#include <config/LocationConfig.hpp>

namespace Webserver
{
	class ConfigLocationParser : public AConfigParser
	{
	public:
		ConfigLocationParser(StreamData streamData);

	protected:
		std::map<std::string, ICommand*> initKeywords();

	private:
		Webserver::LocationConfig _data;
	};
}
