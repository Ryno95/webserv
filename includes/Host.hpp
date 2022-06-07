#pragma once

#include <string>

#include <config/ServerConfig.hpp>
#include <config/LocationConfig.hpp>

namespace Webserver
{
	struct Host : private LocationConfig
	{
		static Host		determine(const ServerConfig& config, const std::string& hostName, const std::string& uri);

		std::string		getName() const;
		bool			getAutoIndexEnabled() const;
		std::string		getRoot() const;
		std::string		getDefaultIndex() const;
		std::string		getDefaultError() const;
		bool			isMethodAccepted(Method::method) const;
		bool			isRedirect() const;
		bool			isUpload() const;
		bool			isChangeRoot() const;
		bool			isCgi() const;

	private:
		static const HostConfig& matchHost(const std::vector<HostConfig>& hosts, const std::string& hostName);

		Host(const HostConfig& config, const std::string& uri);
		void matchLocation(const std::string& uri);

		RouteType::RouteType _routeType;
	};
}
