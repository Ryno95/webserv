#include <config/AppConfig.hpp>
#include <defines.hpp>

namespace Webserver
{
	AppConfig::AppConfig() :
		_listenBacklog(42),
		_bufferSize(2048),
		_mimeFilePath("mime.config"),
		_debugEnabled(false),
		_loggingEnabled(false),
		_logFile("logs.log")
	{
	}

	AppConfig::AppConfig(const AppConfig& ref)
	{
		*this = ref;
	}

	AppConfig::~AppConfig()
	{
	}

	AppConfig& AppConfig::operator=(const AppConfig& ref)
	{
		_listenBacklog = ref._listenBacklog;
		_bufferSize = ref._bufferSize;
		_mimeTypes = ref._mimeTypes;
		_debugEnabled = ref._debugEnabled;
		_loggingEnabled = ref._loggingEnabled;
		_logFile = ref._logFile;
		return *this;
	}

	// AConfig::map_type AppConfig::fillVariablesMap()
	// {
	// 	map_type map;
	// 	map["listen_backlog"]	= var_data(var_uint, &listenBacklog);
	// 	map["buffer_size"]		= var_data(var_uint, &bufferSize);
	// 	map["mime_config"]		= var_data(var_string, &mimeFilePath);
	// 	map["debug_mode"]		= var_data(var_bool, &debugEnabled);
	// 	map["debug_logging"]	= var_data(var_bool, &loggingEnabled);
	// 	map["log_file"]			= var_data(var_string, &logFile);
	// 	return map;
	// }

	// void AppConfig::validate() const
	// {
	// 	if (listenBacklog <= 0)
	// 		throw InvalidValueException("application, listen_backlog should be bigger than 0");
	// 	else if (bufferSize <= 0|| bufferSize >= BODY_SIZE_LIMIT)
	// 		throw InvalidValueException("application, buffer_size should be between 1 and body_size_limit");
	// }
}

