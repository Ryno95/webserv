#pragma once

#include <string>
#include <Method.hpp>

namespace Util
{
	std::string 	removeTrailingWhitespace(const std::string &str);
	std::string 	removeLeadingWhitespace(const std::string &str);
	std::string 	getTimeStamp();
	Method::method	parseMethod(const std::string &method);
	bool			isTerminatorStr(const std::string &str);
}
