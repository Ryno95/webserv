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
}

