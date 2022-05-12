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

namespace TargetType
{
	enum TargetType
	{
		SET_ROOT,
		REDIRECT
	};
}

namespace Webserver
{
	struct LocationConfig : public AConfig
	{
		LocationConfig(); 
		LocationConfig(const LocationConfig& rhs);
		~LocationConfig();
		LocationConfig& operator=(const LocationConfig& rhs);

		map_type fillVariablesMap();
		void validate() const;
		TargetType::TargetType getTargetType() const;
		MatchType::MatchType getMatchType() const;

		std::string	pattern;
		std::string	root;
		std::string	redirect;

		MatchType::MatchType	matchType;
		TargetType::TargetType	targetType;
	};
}
