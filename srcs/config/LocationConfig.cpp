#include <config/LocationConfig.hpp>

namespace Webserver
{
	LocationConfig::LocationConfig() : AConfig::AConfig(fillVariablesMap())
	{
	}

	LocationConfig::LocationConfig(const std::string& value) : AConfig::AConfig(fillVariablesMap())
	{
		parseRouteType(value);
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
		route = ref.route;
		routeType = ref.routeType;
		return *this;
	}

	AConfig::map_type LocationConfig::fillVariablesMap()
	{
		map_type map;
		map["pattern"]	= var_data(var_string, &pattern);
		map["route"]	= var_data(var_string, &route);
		return map;
	}

	bool LocationConfig::isRedirect() const
	{
		return routeType == RouteType::REDIRECT;
	}

	bool LocationConfig::isUpload() const
	{
		return routeType == RouteType::UPLOAD;
	}

	bool LocationConfig::isChangeRoot() const
	{
		return routeType == RouteType::CHANGE_ROOT;
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
		if (route.size() == 0)
			throw InvalidValueException("A route destination should be set for a location");
	}

	void LocationConfig::parseRouteType(const std::string& value)
	{
		std::string type = value.substr(std::min(value.find(' '), value.find('\t')), std::string::npos);
		type = removeLeadingWhitespace(type);

		if (type == "root")
			routeType = RouteType::CHANGE_ROOT;
		else if (type == "redirect")
			routeType = RouteType::REDIRECT;
		else if (type == "upload")
			routeType = RouteType::UPLOAD;
		else
			throw InvalidValueException("Either root, redirect or upload should be set for a location");
	}
}