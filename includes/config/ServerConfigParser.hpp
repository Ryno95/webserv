#pragma once

#include <config/AParseTreeBranch.hpp>
#include <config/ServerConfig.hpp>
#include <config/HostConfigParser.hpp>

namespace Webserver
{
	class ServerConfigParser :  public AParseTreeBranch<ServerConfig, HostConfigParser>
	{
	public:
		ServerConfigParser(StreamData* streamData);

		void validate();
		std::map<std::string, ICommand*> createKeywords();
	};
}
