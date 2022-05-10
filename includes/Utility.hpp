#pragma once

#include <string>
#include <sstream>

namespace Util
{
	std::string removeTrailingWhitespace(const std::string &str);
	std::string removeLeadingWhitespace(const std::string &str);

	template <typename T>
	std::string toString (T val)
	{
		std::ostringstream ss;
		ss << val;
		return ss.str();
	}
}
