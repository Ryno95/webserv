#pragma once
#include <AConfigParser.hpp>

namespace Webserver
{
	class ConfigParser : public ConfigFileParser::AConfigParser
	{
	public:
		static ConfigParser* parse(const std::string& configFilePath);
	private:
		ConfigParser(std::ifstream* stream);
	protected:
		std::map<std::string, ICommand*> initKeywords();
	};
}
