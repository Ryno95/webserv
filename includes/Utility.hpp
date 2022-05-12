#pragma once

#include <string>
#include <sstream>
#include <Method.hpp>

namespace Webserver
{
	std::string 	removeTrailingWhitespace(const std::string &str);
	std::string 	removeLeadingWhitespace(const std::string &str);
	std::string 	getTimeStamp();
	Method::method	parseMethod(const std::string &method);
	bool			isTerminatorStr(const std::string &str);
	std::string		stringToLower(std::string str);

	template <typename T>
	std::string toString (T val)
	{
		std::ostringstream ss;
		ss << val;
		return ss.str();
	}
}
