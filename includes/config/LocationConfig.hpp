#pragma once

#include <string>

#include <config/AConfig.hpp>

namespace MatchType
{
	enum MatchType
	{
		PREFIX,
		SUFFIX
	};
}

namespace RouteType
{
	enum RouteType
	{
		CHANGE_ROOT,
		REDIRECT,
		UPLOAD
	};
}

namespace Webserver
{
	struct LocationConfig : public AConfig
	{
		LocationConfig();
		LocationConfig(const std::string& value);
		LocationConfig(const LocationConfig& rhs);
		~LocationConfig();
		LocationConfig& operator=(const LocationConfig& rhs);

		map_type fillVariablesMap();
		void validate() const;
		MatchType::MatchType getMatchType() const;

		bool isRedirect() const;
		bool isUpload() const;
		bool isChangeRoot() const;

		std::string	pattern;
		std::string route;
		RouteType::RouteType routeType;

	private:
		void parseRouteType(const std::string& value);
	};
}
