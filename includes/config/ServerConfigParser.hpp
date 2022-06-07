#pragma once

#include <config/ServerConfig.hpp>
#include <config/AConfigParser.hpp>

namespace Webserver
{
	class ServerConfigParser : public AConfigParser
	{
	public:
		ServerConfigParser(StreamData* streamData);

	private:
		std::map<std::string, ICommand*> createKeywords();
		void validate();

		ServerConfig _data;
	};
}
