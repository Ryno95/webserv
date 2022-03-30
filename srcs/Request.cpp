#include <defines.hpp>
#include <Request.hpp>

#include <iostream>

#include <cctype>
#include <cstdlib>

Request::Request(std::string query) : _query(query)
{
	parse();
}

Request::~Request()
{
}

httpStatusCode Request::parseBody()
{
}

// request-line = method SP request-target SP HTTP-version CRLF
httpStatusCode Request::parseRequestLine()
{
	size_t pos, pos2, len;
	int major, minor;
	std::string tmp;

	len = _query.size();

// Get method
	pos = _query.find(' ');
	if (pos == std::string::npos)
		return BAD_REQUEST;
	std::string method = _query.substr(0, pos);
	if (method == "GET")
		_method = GET;
	else if (method == "POST")
		_method = POST;
	else if (method == "DELETE")
		_method = DELETE;
	else
		return BAD_REQUEST;

// Get target
	++pos;
	pos2 = _query.find(' ', pos);
	if (pos == std::string::npos || pos2 - pos == 0)
		return BAD_REQUEST;
	_target = _query.substr(pos, (pos2 - pos));
	if (_target.size() > MAX_TARGET_LEN)
		return URI_TOO_LONG;

// Check version
	++pos2;
	if (_query.substr(pos2, 5) != "HTTP/") // case insensitive?
		return BAD_REQUEST;
	pos = pos2 + 5;

	pos2 = _query.find('.', pos);
	if (pos2 == std::string::npos)
		return BAD_REQUEST;

	tmp = _query.substr(pos, pos2 - pos);  // check if this section (major) ONLY contains digits
	for (size_t i = 0; i < tmp.size(); i++)
	{
		if (!std::isdigit(tmp[i]))
			return BAD_REQUEST;
	}
	major = std::atoi(tmp.c_str());
	if (major != HTTPVERSION_MAJOR)
		return BAD_REQUEST;

	pos = _query.find('\r', 0);
	if (_query.find('\n', 0) - pos != 1) // there's no \n somewhere in the line, only at \r\n
		return BAD_REQUEST;

	tmp = _query.substr(pos2 + 1, pos - pos2 - 1); // check if this section (minor) ONLY contains digits
	for (size_t i = 0; i < tmp.size(); i++)
	{
		if (!std::isdigit(tmp[i]))
			return BAD_REQUEST;
	}
	minor = std::atoi(tmp.c_str());
	if (minor != HTTPVERSION_MINOR)
		return BAD_REQUEST;

	return OK;
}

httpStatusCode Request::parseHeaderFields()
{
}

void Request::parse()
{
	parseRequestLine();
	parseHeaderFields();
	parseBody();
}
