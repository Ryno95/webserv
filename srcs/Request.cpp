#include <Request.hpp>

Request::Request(std::string request) : _request(request)
{
	(void)_method;
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
}
