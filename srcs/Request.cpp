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

httpStatusCode Request::parseBody()
{
}

// request-line = method SP request-target SP HTTP-version CRLF
httpStatusCode Request::parseRequestLine()
{
}


// User-Agent: curl/7.16.3 libcurl/7.16.3 OpenSSL/0.9.7l zlib/1.2.3
// Host: www.example.com
// Accept-Language: en, mi 

httpStatusCode Request::parseHeaderFields()
{

}

httpStatusCode Request::parse()
{
	parseRequestLine();
	parseHeaderFields();
	parseBody();
}
