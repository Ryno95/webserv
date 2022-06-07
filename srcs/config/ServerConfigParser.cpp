#include <config/ServerConfigParser.hpp>
#include <config/HostConfigParser.hpp>

namespace Webserver
{
	ServerConfigParser::ServerConfigParser(StreamData* streamData) : AConfigParser::AConfigParser(streamData)
	{
	}

	std::map<std::string, AConfigParser::ICommand*> ServerConfigParser::initKeywords()
	{
		std::map<std::string, ICommand*> keywords;
		keywords["host"]		= new CreateChildCommand<HostConfigParser>(this);

		keywords["port"]		= new ParseVariableCommand<uint>(&_data._port);
		return keywords;
	}
}
