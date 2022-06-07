#pragma once

#include <config/ServerConfig.hpp>
#include <config/AConfigParser.hpp>

namespace Webserver
{
	class ConfigServerParser : public AConfigParser
	{
	public:
		ConfigServerParser(StreamData streamData);

	protected:
		std::map<std::string, ICommand*> initKeywords();

	private:
		Webserver::ServerConfig _data;
	};
}
