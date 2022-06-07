#include <config/LocationConfig.hpp>

namespace Webserver
{
	LocationConfig::LocationConfig()
	{
	}

	LocationConfig::LocationConfig(const std::string& value)
	{
		// parseRouteType(value);
	}

	LocationConfig::LocationConfig(const LocationConfig& ref)
	{
		*this = ref;
	}

	LocationConfig::~LocationConfig()
	{
	}

	LocationConfig& LocationConfig::operator=(const LocationConfig& ref)
	{
		_pattern = ref._pattern;
		return *this;
	}
}
