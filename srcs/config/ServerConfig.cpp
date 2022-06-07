#include <config/ServerConfig.hpp>
#include <Exception.hpp>

namespace Webserver
{
	ServerConfig::ServerConfig() :
		_port(80)
	{
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
		_port = ref._port;
		_hosts = ref._hosts;
		return *this;
	}

	// AConfig::map_type ServerConfig::fillVariablesMap()
	// {
	// 	map_type map;

	// 	map["listen_port"]		= var_data(var_uint, &port);

	// 	return map;
	// }

	// void ServerConfig::validate() const
	// {
	// 	if (port > _maxPortNumber || port <= 0)
	// 		throw InvalidValueException("server, port should be between 1 and 65535");
	// }
}
