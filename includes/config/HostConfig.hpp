#pragma once

#include <string>
#include <vector>

#include <Method.hpp>

namespace Webserver
{
	class LocationConfig;

	class HostConfig
	{
		friend class ConfigHostParser;

	public:
		HostConfig();
		HostConfig(const HostConfig& ref);
		~HostConfig();
		HostConfig& operator=(const HostConfig& ref);

	private:
		std::vector<std::string>	_names;
		std::vector<LocationConfig>	_locations;

	protected:
		bool						_autoIndexEnabled;
		std::string					_root;
		std::string					_defaultIndex;
		std::string					_defaultError;
		std::vector<Method::method>	_acceptedMethods;
	};
}

#include <config/LocationConfig.hpp> // Placed here instead of at the top because of circular dependency with inheritance dependency in LocationConfig
