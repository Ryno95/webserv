#include <Host.hpp>
#include <config/HostConfig.hpp>
#include <config/ServerConfig.hpp>

#include <algorithm>

namespace Webserver
{
	Host Host::determine(const ServerConfig& config, const std::string& hostName, const std::string& uri)
	{
		return Host(matchHost(config.hosts, hostName), uri);
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

	Host::Host(const HostConfig& config, const std::string& uri)
		: HostConfig::HostConfig(config), _routeType(RouteType::CHANGE_ROOT)
	{
		matchLocation(uri);
	}

	void Host::matchLocation(const std::string& uri)
	{
		for (size_t i = 0; i < locations.size(); i++)
		{
			if (wildcard(uri, locations[i].pattern) == true)
			{
				_routeType = locations[i].routeType;
				root = locations[i].route;
				break;
			}
		}
	}

	std::string Host::getName() const
	{
		return names[0];
	}

	bool Host::getAutoIndexEnabled() const
	{
		return autoIndexEnabled;
	}

	std::string Host::getRoot() const
	{
		return root;
	}

	std::string Host::getDefaultIndex() const
	{
		return defaultIndex;
	}

	std::string Host::getDefaultError() const
	{
		return defaultError;
	}

	bool Host::isMethodAccepted(Method::method m) const
	{
		if (std::find(acceptedMethods.begin(), acceptedMethods.end(), m) == acceptedMethods.end())
			return false;
		return true;
	}

	bool Host::isRedirect() const
	{
		return _routeType == RouteType::REDIRECT;
	}

	bool Host::isUpload() const
	{
		return _routeType == RouteType::UPLOAD;
	}

	bool Host::isChangeRoot() const
	{
		return _routeType == RouteType::CHANGE_ROOT;
	}
}
