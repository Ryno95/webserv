#pragma once

#include <config/ServerConfig.hpp>
#include <config/AConfigParser.hpp>

namespace Webserver
{
	class ServerConfigParser : public AConfigParser
	{
	public:
		ServerConfigParser(StreamData* streamData);

	protected:
		std::map<std::string, ICommand*> createKeywords();

	private:
		Webserver::ServerConfig _data;
	};
}
