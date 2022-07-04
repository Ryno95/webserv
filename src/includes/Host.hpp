#pragma once

#include <string>

#include <Method.hpp>
#include <config/ServerConfig.hpp>
#include <config/LocationConfig.hpp>

namespace Webserver
{
	class Host : public LocationConfig
	{
	public:
		static Host		determine(const ServerConfig& config, const std::string& hostName, const std::string& uri);

		bool isMethodAllowed(Method::method method) const;

	private:
		static const HostConfig& matchHost(const std::vector<HostConfig*>& hosts, const std::string& hostName);
		static LocationConfig matchLocation(const HostConfig& host, const std::string& uri);

		Host(const LocationConfig& config);
	};
}
