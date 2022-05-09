#include <GlobalConfig.hpp>

GlobalConfig::GlobalConfig() : AConfig::AConfig()
{
	fillVariablesMap();
}

GlobalConfig::GlobalConfig(const GlobalConfig& rhs)
{
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
	return *this;
}

AConfig::map_type GlobalConfig::fillVariablesMap()
{
	map_type map;
	map["listen_backlog"] = var_data(var_uint, &listenBacklog);
	map["buffer_size"] = var_data(var_uint, &bufferSize);
	map["mime_config"] = var_data(var_string, &mimeFilePath);
	map["debug_mode"] = var_data(var_bool, &debugEnabled);
	map["debug_logging"] = var_data(var_bool, &loggingEnabled);
	return map;
}
