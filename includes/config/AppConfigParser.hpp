#pragma once

#include <config/AParseTreeBranch.hpp>
#include <config/AppConfig.hpp>
#include <config/ServerConfigParser.hpp>

namespace Webserver
{
	class AppConfigParser : public AParseTreeBranch<AppConfig, ServerConfigParser>
	{
	public:
		typedef AppConfig data_type;

		AppConfigParser(StreamData* streamData);

		std::map<std::string, ICommand*> createKeywords();
		void validate();
	};
}
