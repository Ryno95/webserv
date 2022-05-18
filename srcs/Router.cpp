#include <Router.hpp>
#include <ServerHandler.hpp>

namespace Webserver
{
	Router::Router(const ServerConfig& config) : _config(config)
	{
	}

	Router::~Router()
	{
	}

	HostConfig Router::getHost(const std::string& uri) const
	{
		HostConfig host = matchHost(uri);
		return matchLocation(host);
	}

	const HostConfig& Router::matchHost(const std::string& uri) const
	{
		const std::vector<HostConfig>& hosts = _config.hosts;
		for (size_t i = 0; i < hosts.size(); i++)
		{
			for (size_t j = 0; j < hosts[i].names.size(); j++)
			{
				if (uri == hosts[i].names[j])
					return hosts[i];
			}
		}
		return hosts[0];
	}

	HostConfig Router::matchLocation(HostConfig host) const
	{
		return host;
	}
}
