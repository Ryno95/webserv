#pragma once

#include <config/AParseTreeBranch.hpp>
#include <config/AppConfig.hpp>
#include <config/ServerConfigParser.hpp>

namespace Webserver
{
	class ConfigParser : public AParseTreeBranch<AppConfig, ServerConfigParser>
	{
	public:
		ConfigParser(std::istream& stream);
		~ConfigParser();
		void parse();

		AppConfig* getData() const;

	private:
		std::map<std::string, ICommand*> createKeywords();
		void validate();
	};
}
