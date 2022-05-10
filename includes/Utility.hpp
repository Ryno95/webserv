#pragma once

#include <string>

namespace Util
{
	std::string removeTrailingWhitespace(const std::string &str);
	std::string removeLeadingWhitespace(const std::string &str);
	std::string getTimeStamp();
	bool	isTerminatorStr(const std::string &str);
}
