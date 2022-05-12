#include <iostream>
#include <Method.hpp>
#include <Utility.hpp>
#include <cctype>

namespace Webserver
{
	std::string removeLeadingWhitespace(const std::string &str)
	{
		std::string out = str;
		std::string::const_iterator iter = str.begin();
		std::string::const_iterator end = str.end();
		size_t count = 0;

		while (iter != end)
		{
			if (*iter == ' ' || *iter == '\t')
				count++;
			else
				break;
			iter++;
		}

		out.erase(0, count);
		return out;
	}

	std::string removeTrailingWhitespace(const std::string &str)
	{
		std::string out = str;
		std::string::const_reverse_iterator iter = str.rbegin();
		std::string::const_reverse_iterator end = str.rend();
		size_t count = 0;

		while (iter != end)
		{
			if (*iter == ' ' || *iter == '\t')
				count++;
			else
				break;
			iter++;
		}

		out.erase(out.size() - count, count);
		return out;
	}

	std::string getTimeStamp()
	{
		std::string timestamp;
		struct tm* timeinfo;
		time_t rawtime;

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		timestamp = "[";
		timestamp += asctime(timeinfo);
		timestamp.pop_back();
		timestamp += "] ";
		return timestamp;
	}

	bool	isTerminatorStr(const std::string &str)
	{
		const std::string	terminatorStr = "\r\n\r\n";

		return (str.compare(terminatorStr) == 0);
	}

	Method::method	parseMethod(const std::string &method)
	{
		if (method == "GET")
			return Method::GET;
		else if (method == "POST")
			return Method::POST;
		else if (method == "DELETE")
			return Method::DELETE;
		else
			return Method::INVALID;
	}

	std::string stringToLower(std::string str)
	{
		size_t size = str.size();
		for (size_t i = 0; i < size; i++)
		{
			str[i] = tolower(str[i]);
		}
		return str;
	}
}
