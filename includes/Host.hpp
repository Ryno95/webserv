#pragma once

#include <string>

#include <config/ServerConfig.hpp>
#include <config/LocationConfig.hpp>

namespace Webserver
{
	struct Host : public LocationConfig
	{
		static Host		determine(const ServerConfig& config, const std::string& hostName, const std::string& uri);

		std::string		getName() const;

	private:
		static const HostConfig& matchHost(const std::vector<HostConfig>& hosts, const std::string& hostName);
		static const LocationConfig& matchLocation(const HostConfig& host, const std::string& uri);

		Host(const HostConfig& config);
	};
}
