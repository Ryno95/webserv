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
		AppConfig* parse();

	private:
		std::map<std::string, ICommand*> createKeywords();
		void validate();
		AppConfig* combineParsers() const;

		AppConfig _data;
	};
}
