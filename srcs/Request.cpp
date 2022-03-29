#include <Request.hpp>

#include <iostream>
#include <cctype>

Request::Request(std::string query) : _query(query)
{
	// std::cout << query << std::endl;
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


// User-Agent: curl/7.16.3 libcurl/7.16.3 OpenSSL/0.9.7l zlib/1.2.3
// Host: www.example.com
// Accept-Language: en, mi 

httpStatusCode	addKeyValuePair(const std::string &src, size_t newLinePos,
						std::map<std::string, std::string>*headerFields)
{
	size_t colonPos = src.find(COLON);
	if (colonPos == std::string::npos || isspace(src[colonPos - 1]))
		return BAD_REQUEST;

	std::string key = src.substr(0, colonPos);
	std::string value = src.substr(colonPos + 2, newLinePos);
	headerFields->insert(std::pair<std::string, std::string>(key, value));
	return OK;
}

httpStatusCode Request::parseHeaderFields()
{
	size_t				next = 0, last = 0;
	httpStatusCode		ret;

	while ((next = _query.find(NEW_LINE, last)) != std::string::npos)
	{
		ret = addKeyValuePair(_query.substr(last, next - last), next, &_headerFields);
		if (ret != OK)
			return ret;
		last = next + 1;
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
