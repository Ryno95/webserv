#pragma once

#include <string>

#include <config/HostConfig.hpp>

// namespace MatchType
// {
// 	enum MatchType
// 	{
// 		PREFIX,
// 		SUFFIX
// 	};
// }

// namespace RouteType
// {
// 	enum RouteType
// 	{
// 		CHANGE_ROOT,
// 		REDIRECT,
// 		UPLOAD,
// 		CGI
// 	};
// }

namespace Webserver
{
	class LocationConfig : public HostConfig
	{
		friend class LocationConfigParser;

	public:
		LocationConfig();
		LocationConfig(const std::string& value);
		LocationConfig(const LocationConfig& rhs);
		~LocationConfig();
		LocationConfig& operator=(const LocationConfig& rhs);

		const std::string& getMatchPattern() const;

	private:
		std::string _pattern;
	};
}
