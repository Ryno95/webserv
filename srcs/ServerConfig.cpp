#include <ServerConfig.hpp>

ServerConfig::ServerConfig()
{
	fillVariablesMap();
}

ServerConfig::ServerConfig(const ServerConfig& ref)
{
	*this = ref;
}

ServerConfig::~ServerConfig()
{
}

ServerConfig& ServerConfig::operator=(const ServerConfig& ref)
{
	port = ref.port;
	bufferSize = ref.bufferSize;
	listenBacklog = ref.listenBacklog;
	mimeTypesFile = ref.mimeTypesFile;
	hosts = ref.hosts;
	return *this;
}

AConfig::map_type ServerConfig::fillVariablesMap()
{
	map_type map;

	map["listen_port"]		= var_data(var_uint, &port);
	map["buffer_size"]		= var_data(var_uint, &bufferSize);
	map["mime_config"]		= var_data(var_string, &mimeTypesFile);
	map["listen_backlog"]	= var_data(var_uint, &listenBacklog);

	return map;
}
