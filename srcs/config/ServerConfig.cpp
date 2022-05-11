#include <config/ServerConfig.hpp>
#include <Exception.hpp>

namespace Webserver
{
	ServerConfig::ServerConfig() 
	:	AConfig::AConfig(fillVariablesMap()), port(80)
	{
	}

	ServerConfig::ServerConfig(const ServerConfig& ref) : AConfig::AConfig(fillVariablesMap())
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

	void ServerConfig::validateServerConfig()
	{
		if (port > _maxPortNumber || port <= 0)
			throw InvalidValueException("server, port should be between 1 and 65535");
	}
}
