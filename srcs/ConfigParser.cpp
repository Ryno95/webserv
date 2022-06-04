#include <fstream>

#include <ConfigParser.hpp>
#include <ConfigServerParser.hpp>

namespace Webserver
{
	using namespace ConfigFileParser;

	ConfigParser* ConfigParser::parse(const std::string& configFilePath)
	{
		std::ifstream fstream(configFilePath);
		if (fstream.fail())
			throw FileNotFoundException(configFilePath);

		return new ConfigParser(&fstream);
	}
	
	ConfigParser::ConfigParser(std::ifstream* stream) : AConfigParser::AConfigParser(StreamData(stream, 0), initKeywords())
	{
	}

	std::map<std::string, AConfigParser::ICommand*> ConfigParser::initKeywords()
	{
		std::map<std::string, ICommand*> keywords;
		keywords["server"]	= new CreateChildCommand<ConfigServerParser>(this);
		return keywords;
	}
}
