#include <Host.hpp>
#include <config/HostConfig.hpp>
#include <config/ServerConfig.hpp>

#include <algorithm>

namespace Webserver
{
	Host Host::determine(const ServerConfig& config, const std::string& hostName, const std::string& uri)
	{
		const HostConfig& host = matchHost(config.hosts, hostName);
		return Host(matchLocation(host, uri));
	}

	const HostConfig& Host::matchHost(const std::vector<HostConfig>& hosts, const std::string& hostName)
	{
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

	const HostConfig& Host::matchLocation(const HostConfig& host, const std::string& uri)
	{
		const std::vector<LocationConfig>& locations = host.getLocationConfigs();
		for (size_t i = 0; i < locations.size(); i++)
		{
			if (wildcard(uri, locations[i].pattern) == true)
				return locations[i];
		}
		return host;
	}

	Host::Host(const HostConfig& config) : LocationConfig::LocationConfig(config)
	{
	}

	std::string Host::getName() const
	{
		return names[0];
	}

	// bool Host::isMethodAccepted(Method::method m) const
	// {
	// 	if (std::find(acceptedMethods.begin(), acceptedMethods.end(), m) == acceptedMethods.end())
	// 		return false;
	// 	return true;
	// }

	// bool Host::isRedirect() const
	// {
	// 	return _routeType == RouteType::REDIRECT;
	// }

	// bool Host::isUpload() const
	// {
	// 	return _routeType == RouteType::UPLOAD;
	// }

	// bool Host::isChangeRoot() const
	// {
	// 	return _routeType == RouteType::CHANGE_ROOT;
	// }

	// bool Host::isCgi() const
	// {
	// 	return _routeType == RouteType::CGI;
	// }
}
