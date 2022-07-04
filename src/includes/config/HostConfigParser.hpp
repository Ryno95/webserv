#pragma once

#include <config/AParseTreeBranch.hpp>
#include <config/HostConfig.hpp>
#include <config/LocationConfigParser.hpp>

namespace Webserver
{
	class HostConfigParser : public AParseTreeBranch<HostConfig, LocationConfigParser>
	{
	public:
		HostConfigParser(StreamData* streamData);
		void addChild(const std::string& args);

		void validate();
		std::map<std::string, ICommand*> createKeywords();
	};
}
