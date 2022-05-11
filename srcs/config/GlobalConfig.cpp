#include <config/GlobalConfig.hpp>
#include <defines.hpp>

namespace Webserver
{

	#pragma region Static

	GlobalConfig GlobalConfig::_current;

	const GlobalConfig& GlobalConfig::get()
	{
		return _current;
	}

	void GlobalConfig::set(const GlobalConfig& config)
	{
		_current = config;
	}

	#pragma endregion



	GlobalConfig::GlobalConfig()
	:	AConfig::AConfig(fillVariablesMap()),
		listenBacklog(42),
		bufferSize(2048),
		mimeFilePath("mime.config"),
		debugEnabled(false),
		loggingEnabled(false),
		defaultLogFile("logs.log")
	{
	}

	GlobalConfig::GlobalConfig(const GlobalConfig& ref) : AConfig::AConfig(fillVariablesMap())
	{
		*this = ref;
	}

	GlobalConfig::~GlobalConfig()
	{
	}

	GlobalConfig& GlobalConfig::operator=(const GlobalConfig& rhs)
	{
		listenBacklog = rhs.listenBacklog;
		bufferSize = rhs.bufferSize;
		mimeTypes = rhs.mimeTypes;
		debugEnabled = rhs.debugEnabled;
		loggingEnabled = rhs.loggingEnabled;
		defaultLogFile = rhs.defaultLogFile;
		return *this;
	}

	AConfig::map_type GlobalConfig::fillVariablesMap()
	{
		map_type map;
		map["listen_backlog"]	= var_data(var_uint, &listenBacklog);
		map["buffer_size"]		= var_data(var_uint, &bufferSize);
		map["mime_config"]		= var_data(var_string, &mimeFilePath);
		map["debug_mode"]		= var_data(var_bool, &debugEnabled);
		map["debug_logging"]	= var_data(var_bool, &loggingEnabled);
		map["default_log"]		= var_data(var_string, &defaultLogFile);
		return map;
	}

	void GlobalConfig::validateGlobalConfig() const
	{
		if (listenBacklog <= 0)
			throw InvalidValueException("application, listen_backlog should be bigger than 0");
		else if (bufferSize <= 0|| bufferSize >= BODY_SIZE_LIMIT)
			throw InvalidValueException("application, buffer_size should be between 1 and body_size_limit");
	}
}

