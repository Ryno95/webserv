#include <Request.hpp>

#include <iostream>

Request::Request(std::string query) : _query(query)
{
	std::cout << query << std::endl;
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

void Request::parseHeaderFields()
{
}

void Request::parse()
{
	parseRequestLine();
	parseHeaderFields();
	parseBody();
}
