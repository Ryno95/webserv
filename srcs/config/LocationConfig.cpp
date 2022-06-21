#include <config/LocationConfig.hpp>

namespace Webserver
{
	LocationConfig::LocationConfig() :
		HostFields::HostFields(),
		_routeType(RouteType::FILESERVER)
	{
	}

	LocationConfig::LocationConfig(const LocationConfig& ref) : HostFields::HostFields(ref)
	{
		*this = ref;
	}

	LocationConfig::LocationConfig(const HostFields& ref) : HostFields::HostFields(ref),
		_routeType(RouteType::FILESERVER)
	{
		WARN("LocationConfig(HostFields&) called");
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
