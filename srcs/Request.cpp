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

void Request::parseBody()
{
}

// request-line = method SP request-target SP HTTP-version CRLF
void Request::parseRequestLine()
{
}

void	Request::addKeyValuePair(const std::string &src, size_t newLinePos)
{
	const size_t 	colonPos = src.find(COLON);
	int				i = 0;

	if (colonPos == std::string::npos || std::isspace(src[colonPos - 1]))
		throwError(BAD_REQUEST);
	
	std::string key = src.substr(i, colonPos);
	while(std::isspace(src[colonPos + CRLF_CHAR_COUNT + i]))
		++i;
	std::string value = src.substr(colonPos + CRLF_CHAR_COUNT + i, newLinePos);
	_headerFields.insert(std::pair<std::string, std::string>(key, value));
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

void Request::parseHeaderFields()
{
	size_t				next = 0, last = 0;
	std::string			trimmedLine;

	while ((next = _query.find(CRLF, last)) != std::string::npos)
	{
		trimmedLine = getTrimmedLine(_query.substr(last, next - last));
		addKeyValuePair(trimmedLine, next);
		last = next + CRLF_CHAR_COUNT;
	}
}

void Request::parse()
{
	parseRequestLine();
	parseHeaderFields();
	parseBody();
}

void Request::throwError(httpStatusCode code)
{
	_status = code;
	throw std::runtime_error("Parse error");
}


httpStatusCode Request::getStatus() const
{
	return _status;
}
