#include <config/LocationConfig.hpp>

namespace Webserver
{
	LocationConfig::LocationConfig() : AConfig::AConfig(fillVariablesMap())
	{
	} 

	LocationConfig::LocationConfig(const LocationConfig& ref) : AConfig::AConfig(fillVariablesMap())
	{
		*this = ref;
	}

	LocationConfig::~LocationConfig()
	{
	}

	LocationConfig& LocationConfig::operator=(const LocationConfig& ref)
	{
		pattern = ref.pattern;
		root = ref.root;
		redirect = ref.redirect;
		targetType = ref.targetType;
		matchType = ref.matchType;
		return *this;
	}

	AConfig::map_type LocationConfig::fillVariablesMap()
	{
		map_type map;
		map["location"]	= var_data(var_string, &pattern);
		map["root"]		= var_data(var_string, &root);
		map["redirect"]	= var_data(var_string, &redirect);
		return map;
	}

	TargetType::TargetType LocationConfig::getTargetType() const
	{
		if (root.size() != 0)
			return TargetType::SET_ROOT;
		else
			return TargetType::REDIRECT;
	}

	/*
		Returns MatchType::SUFFIX or MatchType::PREFIX.
		SUFFIX means matching based on "ends with"
		PREFIX means matching based on "begins with"
	*/
	MatchType::MatchType LocationConfig::getMatchType() const
	{
		if (pattern[0] == '*')
			return MatchType::SUFFIX;
		else
			return MatchType::PREFIX;
	}

	void LocationConfig::validate() const
	{
		if ((root.size() == 0 && redirect.size() == 0) || (root.size() != 0 && redirect.size() != 0))
			throw InvalidValueException("Either root or redirect should be set for a location");
	}
}
