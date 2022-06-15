#include <config/LocationConfig.hpp>

namespace Webserver
{
	LocationConfig::LocationConfig() :
		_routeType(RouteType::FILESERVER)
	{
	}

	LocationConfig::LocationConfig(const LocationConfig& ref)
	{
		*this = ref;
	}

	LocationConfig::LocationConfig(const HostConfig& ref) : HostConfig::HostConfig(ref),
		_routeType(RouteType::FILESERVER)
	{
	}

	LocationConfig::~LocationConfig()
	{
	}

	LocationConfig& LocationConfig::operator=(const LocationConfig& ref)
	{
		_pattern = ref._pattern;
		_routeType = ref._routeType;
		return *this;
	}

	const std::string& LocationConfig::getMatchPattern() const
	{
		return _pattern;
	}

	RouteType::RouteType LocationConfig::getRouteType() const
	{
		return _routeType;
	}
}
