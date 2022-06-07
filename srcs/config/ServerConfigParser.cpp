#include <config/ServerConfigParser.hpp>
#include <config/HostConfigParser.hpp>

namespace Webserver
{
	ServerConfigParser::ServerConfigParser(StreamData* streamData) : AConfigParser::AConfigParser(streamData)
	{
	}

	std::map<std::string, AConfigParser::ICommand*> ServerConfigParser::createKeywords()
	{
		std::map<std::string, ICommand*> keywords;
		keywords["host"]		= new CreateChildCommand<HostConfigParser>(this);

		keywords["port"]		= new ParseVariableCommand<uint>(&_data._port);
		return keywords;
	}

	void ServerConfigParser::validate()
	{
		if (_children.size() == 0)
			throw ParseException("No hosts configured.");
		if (_data._port > UINT16_MAX)
			throw InvalidValueException(std::string("Port: " + toString(_data._port)));
	}
}
