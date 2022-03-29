#include <Request.hpp>

#include <iostream>

Request::Request(std::string query) : _query(query)
{
	std::cout << query << std::endl;
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
	size_t pos, pos2, len;

	len = _query.size();

// Get method
	pos = _query.find(' ');
	if (pos == std::string::npos)
		return;
	std::string method = _query.substr(0, pos);
	if (method == "GET")
		_method = GET;
	else if (method == "POST")
		_method = POST;
	else if (method == "DELETE")
		_method = DELETE;
	else
		return;

// Get target
	++pos;
	pos2 = _query.find(' ', pos);
	if (pos == std::string::npos)
		return;
	_target = _query.substr(pos, (pos2 - pos));

// Get version
	++pos2;
	pos = _query.find('\r', pos2);
	if (pos == std::string::npos)
		return;
	_version = _query.substr(pos2, pos - pos2);

	std::cout << "Method: " << method << "(" << _method << ")" << std::endl;
	std::cout << "Target: " << _target << std::endl;
	std::cout << "Version: " << _version << std::endl;
}

void Request::parseHeaderFields()
{
}

void Request::parse()
{
	parseRequestLine();
	parseHeaderFields();
	parseBody();
}
