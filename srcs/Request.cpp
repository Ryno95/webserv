#include <Request.hpp>

#include <iostream>
#include <cctype>

Request::Request(std::string query) : _query(query)
{
	(void)_method;
	parse();
}

Request::~Request()
{
}

httpStatusCode Request::parseBody()
{
	return OK;
}

// request-line = method SP request-target SP HTTP-version CRLF
httpStatusCode Request::parseRequestLine()
{
	return OK;
}

httpStatusCode	addKeyValuePair(const std::string &src, size_t newLinePos,
						std::map<std::string, std::string>*headerFields)
{
	const size_t 	colonPos = src.find(COLON);
	int				i = 0;

	if (colonPos == std::string::npos || std::isspace(src[colonPos - 1]))
		return BAD_REQUEST;

	// while(std::isspace(src[i]))
	// 	++i;
	std::string key = src.substr(i, colonPos);

	i = 0;
	while(std::isspace(src[colonPos + CRLF_CHAR_COUNT + i]))
		++i;
	// std::cout << "POS BEFORE: " << newLinePos;
	// while(std::isspace(src[newLinePos - 1]))
	// 	--newLinePos;
	// std::cout << "   POS AFTER: " << newLinePos << std::endl;

	std::string value = src.substr(colonPos + CRLF_CHAR_COUNT + i, newLinePos);
	// std::cout << "VALUE: " << value << std::endl;
	headerFields->insert(std::pair<std::string, std::string>(key, value));
	return OK;
}

std::string	getTrimmedLine(std::string line)
{
	std::string::iterator start = line.begin();
    std::string::iterator end = line.end();

	while (start != end && std::isspace(*start))
		++start;
	--end;
	while (std::distance(start, end) > 0 && std::isspace(*end))
		--end;
	return std::string(start, end + 1);
}

httpStatusCode Request::parseHeaderFields()
{
	size_t				next = 0, last = 0;
	httpStatusCode		ret;
	std::string			trimmedLine;

	while ((next = _query.find(CRLF, last)) != std::string::npos)
	{
		trimmedLine = getTrimmedLine(_query.substr(last, next - last));
		ret = addKeyValuePair(trimmedLine, next, &_headerFields);
		if (ret != OK)
			return ret;
		last = next + CRLF_CHAR_COUNT;
	}
	return OK;
}

httpStatusCode Request::parse()
{
	parseRequestLine();
	parseHeaderFields();
	parseBody();
	return OK;
}
