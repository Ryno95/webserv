#include <fstream>

#include <config/ConfigParser.hpp>
#include <config/ServerConfigParser.hpp>

namespace Webserver
{
	ConfigParser::ConfigParser(std::istream& stream) : AConfigParser::AConfigParser(new StreamData(stream, 1), initKeywords())
	{
	}

	ConfigParser::~ConfigParser()
	{
		delete _streamData;
	}

	std::map<std::string, AConfigParser::ICommand*> ConfigParser::initKeywords()
	{
		std::map<std::string, ICommand*> keywords;
		keywords["server"]	= new CreateChildCommand<ServerConfigParser>(this);

		keywords["listen_backlog"]	= new ParseVariableCommand<uint>(&_data._listenBacklog);
		keywords["buffer_size"]		= new ParseVariableCommand<uint>(&_data._bufferSize);
		keywords["mime_config"]		= new ParseVariableCommand<std::string>(&_data._mimeFilePath);
		keywords["debug_mode"]		= new ParseVariableCommand<bool>(&_data._debugEnabled);
		keywords["debug_logging"]	= new ParseVariableCommand<bool>(&_data._loggingEnabled);
		keywords["log_file"]		= new ParseVariableCommand<std::string>(&_data._logFile);
		return keywords;
	}

	void ConfigParser::parse()
	{
		readStream();
	}
}
