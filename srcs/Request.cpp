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
	size_t colonPos = src.find(COLON);
	if (colonPos == std::string::npos || isspace(src[colonPos - 1]))
		return BAD_REQUEST;

	std::string key = src.substr(0, colonPos);
	std::string value = src.substr(colonPos + CRLF_COUNT, newLinePos);
	headerFields->insert(std::pair<std::string, std::string>(key, value));
	return OK;
}

httpStatusCode Request::parseHeaderFields()
{
	size_t				next = 0, last = 0;
	httpStatusCode		ret;

	while ((next = _query.find(CRLF, last)) != std::string::npos)
	{
		ret = addKeyValuePair(_query.substr(last, next - last), next, &_headerFields);
		if (ret != OK)
			return ret;
		last = next + CRLF_COUNT;
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
