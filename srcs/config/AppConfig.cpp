#include <config/AppConfig.hpp>
#include <defines.hpp>

namespace Webserver
{
	AppConfig::AppConfig() :
		_listenBacklog(42),
		_bufferSize(4096),
		_mimeFilePath("mime.config"),
		_debugEnabled(false),
		_loggingEnabled(false),
		_logFile("logs.log"),
		_maxRequestBodySize(40960)
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

	uint AppConfig::getBufferSize() const
	{
		return _bufferSize;
	}

	const MimeTypes& AppConfig::getMimeTypes() const
	{
		return _mimeTypes;
	}

	uint AppConfig::getListenBacklog() const
	{
		return _listenBacklog;
	}

	bool AppConfig::isDebugEnabled() const
	{
		return _debugEnabled;
	}

	bool AppConfig::isDebugLoggingEnabled() const
	{
		return _loggingEnabled;
	}

	const std::string& AppConfig::getLogFileName() const
	{
		return _logFile;
	}

	uint AppConfig::getMaxRequestBodySize() const
	{
		return _maxRequestBodySize;
	}

}
