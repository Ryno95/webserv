#include <Host.hpp>
#include <config/HostConfig.hpp>
#include <config/ServerConfig.hpp>
#include <Utility.hpp>

#include <algorithm>

namespace Webserver
{
	Host Host::determine(const ServerConfig& config, const std::string& hostName, const std::string& uri)
	{
		const HostConfig& host = matchHost(config.getChildren(), hostName);
		return Host(matchLocation(host, uri));
	}

	const HostConfig& Host::matchHost(const std::vector<HostConfig*>& hosts, const std::string& hostName)
	{
		for (size_t i = 0; i < hosts.size(); i++)
		{
			const std::vector<std::string>& hostNames = hosts[i]->getHostNames();
			for (size_t j = 0; j < hostNames.size(); j++)
			{
				if (stringToLower(hostName) == stringToLower(hostNames[j]))
					return *hosts[i];
			}
		}
		return *hosts[0];
	}

	LocationConfig Host::matchLocation(const HostConfig& host, const std::string& uri)
	{
		const std::vector<LocationConfig*>& locations = host.getChildren();
		for (size_t i = 0; i < locations.size(); i++)
		{
			if (wildcard(uri, locations[i]->getMatchPattern()) == true)
				return *locations[i];
		}
		return LocationConfig(host);
	}

	Host::Host(const LocationConfig& config) : LocationConfig::LocationConfig(config)
	{
	}

	std::string Host::getName() const
	{
		return getHostNames()[0];
	}
}
