#pragma once
#include <config/AConfigParser.hpp>
#include <config/AppConfig.hpp>

namespace Webserver
{
	class ConfigParser : public AConfigParser
	{
	public:
		ConfigParser(std::ifstream* stream);
	protected:
		std::map<std::string, ICommand*> initKeywords();
	private:
		Webserver::AppConfig _data;
	};
}
