#include <fstream>

#include <config/ConfigParser.hpp>
#include <config/ServerConfigParser.hpp>

namespace Webserver
{
	ConfigParser::ConfigParser(std::istream& stream) : AParseTreeBranch::AParseTreeBranch(new StreamData(stream, 1), createKeywords())
	{
	}

	ConfigParser::~ConfigParser()
	{
		delete _streamData;
	}

	std::map<std::string, ICommand*> ConfigParser::createKeywords()
	{
		std::map<std::string, ICommand*> keywords;
		keywords["server"]	= new CreateChildCommand<ServerConfigParser>(this);

		keywords["listen_backlog"]	= new ParseVariableCommand<uint>(&_data->_listenBacklog);
		keywords["buffer_size"]		= new ParseVariableCommand<uint>(&_data->_bufferSize);
		keywords["mime_config"]		= new ParseVariableCommand<std::string>(&_data->_mimeFilePath);
		keywords["debug_mode"]		= new ParseVariableCommand<bool>(&_data->_debugEnabled);
		keywords["debug_logging"]	= new ParseVariableCommand<bool>(&_data->_loggingEnabled);
		keywords["log_file"]		= new ParseVariableCommand<std::string>(&_data->_logFile);
		return keywords;
	}

	void ConfigParser::validate()
	{
		/*
			Rules
		*/

		if (_children.size() == 0)
			throw ParseException("No servers configured.");
		if (_data->_bufferSize == 0)
			throw ParseException("Buffer size must be greater than 0.");


		/*
			Warnings
		*/

		if (_data->_bufferSize > UINT16_MAX)
		{
			WARN("Large buffersize configured: " << _data->_bufferSize);
		}
	}

	AppConfig* ConfigParser::getData() const
	{
		return _data;
	}

	void ConfigParser::parse()
	{
		readStream();
		validate();
	}
}
