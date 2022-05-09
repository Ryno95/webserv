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
	hosts = ref.hosts;
	return *this;
}

AConfig::map_type ServerConfig::fillVariablesMap()
{
	map_type map;

	map["listen_port"]		= var_data(var_uint, &port);

	return map;
}
