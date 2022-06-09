#pragma once

#include <vector>

#include <config/HostConfig.hpp>
#include <config/AComposite.hpp>

namespace Webserver
{
	typedef unsigned int uint;

	class ServerConfig : public AComposite<HostConfig*>
	{
		friend class ServerConfigParser;

	public:
		ServerConfig();
		ServerConfig(const ServerConfig& ref);
		~ServerConfig();
		ServerConfig& operator=(const ServerConfig& ref);

		uint getPort() const;

	private:
		uint _port;
	};
}
