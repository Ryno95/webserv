#pragma once
#include <config/AConfigParser.hpp>
#include <config/AppConfig.hpp>

namespace Webserver
{
	class ConfigParser : public AConfigParser
	{
	public:
		ConfigParser(std::istream& stream);
		~ConfigParser();
		void parse();

	protected:
		std::map<std::string, ICommand*> createKeywords();

	private:
		Webserver::AppConfig _data;
	};
}
