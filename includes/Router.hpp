#pragma once

#include <string>

#include <config/HostConfig.hpp>
#include <PollHandler.hpp>

namespace Webserver
{
	class Router
	{
	public:
		Router(const ServerConfig& config);
		~Router();

		HostConfig getHost(const std::string& hostName, const std::string& target) const;

	private:
		const HostConfig& matchHost(const std::string& hostName) const;
		HostConfig matchLocation(HostConfig host, const std::string& target) const;

		const ServerConfig& _config;
	};
}
