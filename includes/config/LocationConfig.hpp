#pragma once

#include <string>

#include <config/HostConfig.hpp>
#include <RouteType.hpp>

namespace Webserver
{
	class LocationConfig : public HostFields
	{
		friend class LocationConfigParser;

	public:
		LocationConfig();
		LocationConfig(const LocationConfig& rhs);
		LocationConfig(const HostFields& ref);
		~LocationConfig();
		LocationConfig& operator=(const HostFields& ref);
		LocationConfig& operator=(const LocationConfig& rhs);

		RouteType::RouteType getRouteType() const;
		const std::string& getMatchPattern() const;

	private:
		std::string _pattern;
		RouteType::RouteType _routeType;
	};
}
