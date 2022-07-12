#pragma once

#include <string>
#include <sstream>
#include <Method.hpp>

namespace Webserver
{
	std::pair<int, std::string> parseHttpStatusCode(const std::string& statusCode);
	std::string		trimString(const std::string& line);
	std::string 	removeTrailingWhitespace(const std::string &str);
	std::string 	removeLeadingWhitespace(const std::string &str);
	std::string 	getTimeStamp();
	Method::method	parseMethod(const std::string &method);
	bool			isTerminatorStr(const std::string &str);
	std::string		stringToLower(std::string str);
	bool			wildcard(const std::string& string, const std::string& pattern);

	template <typename T>
	std::string toString (T val)
	{
		std::ostringstream ss;
		ss << val;
		return ss.str();
	}

	struct CmpCaseInsensitive
	{
		bool operator()(const std::string& a, const std::string& b) const
		{
			return stringToLower(a) < stringToLower(b);
		}
	};
}
