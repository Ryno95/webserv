#include <Router.hpp>
#include <ServerHandler.hpp>
#include <config/LocationConfig.hpp>

namespace Webserver
{
	Router::Router(const ServerConfig& config) : _config(config)
	{
	}

	Router::~Router()
	{
	}

	HostConfig Router::getHost(const std::string& hostName, const std::string& target) const
	{
		HostConfig hostConfig = matchHost(hostName);
		return matchLocation(hostConfig, target);
	}

	const HostConfig& Router::matchHost(const std::string& hostName) const
	{
		const std::vector<HostConfig>& hosts = _config.hosts;
		for (size_t i = 0; i < hosts.size(); i++)
		{
			for (size_t j = 0; j < hosts[i].names.size(); j++)
			{
				if (stringToLower(hostName) == stringToLower(hosts[i].names[j]))
					return hosts[i];
			}
		}
		return hosts[0];
	}

	HostConfig Router::matchLocation(HostConfig hostConfig, const std::string& target) const
	{
		for (size_t i = 0; i < hostConfig.locations.size(); i++)
		{
			if (wildcard(target, hostConfig.locations[i].pattern) == true)
			{
				hostConfig.root = hostConfig.locations[i].route;
				break;
			}
		}
		return hostConfig;
	}
}
