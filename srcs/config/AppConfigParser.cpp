#include <fstream>

#include <config/AppConfigParser.hpp>
#include <config/ServerConfigParser.hpp>

namespace Webserver
{
	AppConfigParser::AppConfigParser(StreamData* streamData) : AParseTreeBranch::AParseTreeBranch(streamData)
	{
	}

	std::map<std::string, ICommand*> AppConfigParser::createKeywords()
	{
		std::map<std::string, ICommand*> keywords;
		keywords["server"]	= new CreateChildCommand<ServerConfigParser>(this);

		keywords["listen_backlog"]			= new ParseVariableCommand<uint>(&_data->_listenBacklog);
		keywords["buffer_size"]				= new ParseVariableCommand<uint>(&_data->_bufferSize);
		keywords["mime_config"]				= new ParseVariableCommand<std::string>(&_data->_mimeFilePath);
		keywords["debug_mode"]				= new ParseVariableCommand<bool>(&_data->_debugEnabled);
		keywords["debug_logging"]			= new ParseVariableCommand<bool>(&_data->_loggingEnabled);
		keywords["log_file"]				= new ParseVariableCommand<std::string>(&_data->_logFile);
		keywords["max_requestbody_size"]	= new ParseVariableCommand<uint>(&_data->_maxRequestBodySize);
		return keywords;
	}

	void AppConfigParser::validate()
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
}
