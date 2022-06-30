#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include <Exception.hpp>
#include <RouteType.hpp>
#include <config/HostFields.hpp>

namespace Webserver
{
	class VariableParser
	{
	public:
		template<class T>
		T parse(const std::string& value) const;
		~VariableParser() {}
		VariableParser() {}
	};

	template<>
	inline uint VariableParser::parse(const std::string& value) const
	{
		if (value.find_first_not_of("0123456789") != std::string::npos)
			throw Webserver::InvalidValueException(value);
		return std::atoi(value.c_str());
	}

	template<>
	inline std::string VariableParser::parse(const std::string& value) const
	{
		return value;
	}

	template<>
	inline bool VariableParser::parse(const std::string& value) const
	{
		if (value == "true")
			return true;
		else if (value == "false")
			return false;
		else
			throw Webserver::InvalidValueException(value);
	}

	template<>
	inline std::vector<std::string> VariableParser::parse(const std::string& value) const
	{
		std::string str = value;
		std::vector<std::string> values;

		while (true)
		{
			size_t end = std::min(std::min(str.find(' '), str.find('\t')), str.size());
			values.push_back(str.substr(0, end));

			if (end == str.size())
				break;

			str = str.substr(end, str.size() - end);
			str = Webserver::removeLeadingWhitespace(str);
		}
		return values;
	}

	template<>
	inline std::vector<Method::method> VariableParser::parse(const std::string& value) const
	{
		std::string str = value;
		std::vector<Method::method> values;

		while (true)
		{
			size_t end = std::min(std::min(str.find(' '), str.find('\t')), str.size());

			Method::method methodId = Webserver::parseMethod(str.substr(0, end));
			if (methodId == Method::INVALID)
				throw Webserver::InvalidValueException(value);

			values.push_back(methodId);

			if (end == str.size())
				break;

			str = str.substr(end, str.size() - end);
			str = Webserver::removeLeadingWhitespace(str);
		}
		return values;
	}

	template<>
	inline RouteType::RouteType VariableParser::parse(const std::string& value) const
	{
		if (value == "fileserver")
			return RouteType::FILESERVER;
		else if (value == "redirect")
			return RouteType::REDIRECT;
		else if (value == "cgi")
			return RouteType::CGI;
		else
			throw Webserver::InvalidValueException(value);
	}

	template<>
	inline HostFields::ErrorPages VariableParser::parse(const std::string& value) const
	{
		std::string str = value;
		HostFields::ErrorPages pages;

		size_t lastWhitespace = str.rfind(' ');
		size_t lastHorizontalTab = str.rfind('\t');

		if (lastWhitespace == std::string::npos)
			lastWhitespace = lastHorizontalTab;
		else if (lastHorizontalTab != std::string::npos)
			lastWhitespace = std::max(lastWhitespace, lastHorizontalTab);

		std::string page = str.substr(lastWhitespace + 1);
		str = removeTrailingWhitespace(str.substr(0, lastWhitespace));

		while (true)
		{
			size_t end = std::min(std::min(str.find(' '), str.find('\t')), str.size());

			uint statusCode = parse<uint>(str.substr(0, end));

			pages.insert(HostFields::ErrorPage(statusCode, page));

			if (end == str.size())
				break;

			str = str.substr(end, str.size() - end);
			str = removeLeadingWhitespace(str);
		}
		return pages;
	}
}
