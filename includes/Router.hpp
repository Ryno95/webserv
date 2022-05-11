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

		HostConfig getHost(const std::string& uri) const;

	private:
		const HostConfig& matchHost(const std::string& uri) const;
		HostConfig matchLocation(HostConfig host) const;

		const ServerConfig& _config;
	};
}
