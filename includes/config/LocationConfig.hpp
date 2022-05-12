#pragma once

#include <string>

#include <AConfig.hpp>

namespace Webserver
{
	struct LocationConfig : public AConfig
	{
		enum MatchType
		{
			prefix,
			suffix
		};

		enum HandleType
		{
			root,
			redirect
		};

		std::string	pattern;
		std::string	handle;

		MatchType	matchType;
		HandleType	handleType;
	};
}
