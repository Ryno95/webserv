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

		HostConfig* getData() const;
		void validate();

	protected:
		std::map<std::string, ICommand*> createKeywords();
		void addProtectedKeywords(std::map<std::string, ICommand*>& keywords);

	};
}
