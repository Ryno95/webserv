#pragma once

#include <string>

#include <config/ServerConfig.hpp>
#include <config/LocationConfig.hpp>

namespace Webserver
{
	struct Host : public HostConfig
	{
		static Host		determine(const ServerConfig& config, const std::string& hostName, const std::string& uri);

		std::string		getName() const;

		// bool			isMethodAccepted(Method::method) const;
		// bool			isRedirect() const;
		// bool			isUpload() const;
		// bool			isChangeRoot() const;
		// bool			isCgi() const;

	private:
		static const HostConfig& matchHost(const std::vector<HostConfig>& hosts, const std::string& hostName);
		static const LocationConfig& matchLocation(const HostConfig& host, const std::string& uri);

		Host(const HostConfig& config);
	};
}
