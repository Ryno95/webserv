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
}
