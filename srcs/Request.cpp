#include <Request.hpp>

#include <iostream>

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

// Get version
	++pos2;
	pos = _query.find('\r', pos2);
	if (pos == std::string::npos)
		return BAD_REQUEST;
	_version = _query.substr(pos2, pos - pos2);
	if (_version != "http/1.1")
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
