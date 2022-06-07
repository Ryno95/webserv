#pragma once

#include <string>

#include <config/HostConfig.hpp>
#include <RouteType.hpp>

namespace Webserver
{
	class LocationConfig : public HostConfig
	{
		friend class LocationConfigParser;

	public:
		LocationConfig();
		LocationConfig(const LocationConfig& rhs);
		~LocationConfig();
		LocationConfig& operator=(const LocationConfig& rhs);

		const std::string& getMatchPattern() const;
		RouteType::RouteType getRouteType() const;

	private:
		std::string _pattern;
		RouteType::RouteType _routeType;
	};
}
