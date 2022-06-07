#pragma once

#include <vector>

#include <config/HostConfig.hpp>

namespace Webserver
{
	typedef unsigned int uint;

	class ServerConfig
	{
		friend class ServerConfigParser;

	public:
		ServerConfig();
		ServerConfig(const ServerConfig& ref);
		~ServerConfig();
		ServerConfig& operator=(const ServerConfig& ref);

		const std::vector<HostConfig> getHostConfigs() const;
		uint getPort() const;

	private:
		std::vector<HostConfig>	_hosts;

		uint					_port;

		static const int	_maxPortNumber = 65535;
	};
}
