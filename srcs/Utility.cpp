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

		// timestamp = "[";
		timestamp += asctime(timeinfo);
		timestamp.pop_back();
		// timestamp += "] ";
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

	std::string prependRoot(const std::string& root, const std::string& target)
	{
		const size_t	targetSlashPos = target.find('/');
		const size_t	rootSlashPos = root.find_last_of('/');
		const size_t	rootSize = root.size();

		if (target.find(root) != std::string::npos && targetSlashPos == 0)
			return target.substr(1, target.size());
		if (rootSlashPos == rootSize - 1 && targetSlashPos == 0)
			return std::string(root + target.substr(targetSlashPos + 1, target.size()));
		else if ((rootSlashPos == rootSize - 1 && targetSlashPos != 0)
					|| (rootSlashPos != rootSize - 1 && targetSlashPos == 0))
			return std::string(root + target);
		else
			return std::string(root + "/" + target);
	}

	bool wildcard(const std::string& string, const std::string& pattern)
	{
		size_t targetIter = 0, patternIter = 0, end = 0;
		bool inWildcard = false;
		std::string match;

		if (pattern.size() == 0 && string.size() != 0)
			return false;

		while (end != std::string::npos)
		{
			end = pattern.find('*', patternIter);
			if (end == patternIter)
			{
				patternIter++;
				inWildcard = true;
				continue;
			}
			else if (end == std::string::npos)
				inWildcard = true;

			match = pattern.substr(patternIter, end - patternIter);

			if (inWildcard == false)
			{
				if (string.substr(targetIter, match.size()) != match)
					return false;

				targetIter += match.size();
				patternIter += match.size() + 1;
				inWildcard = true;
			}
			else
			{
				if (match.size() > 0)
				{
					size_t pos = string.find(match, targetIter);
					if (pos == std::string::npos)
						return false;
					targetIter += (pos - targetIter);
				}
				else
					return true;

				if (end == std::string::npos)
				{
					if (targetIter + match.size() != string.size())
						return false;
				}

				inWildcard = false;
			}
		}
		return true;
	}
}
