#pragma once

#include <string>
#include <map>

#include <defines.hpp>

class Request
{
#define COLON 		":"
#define CRLF		"\r\n"
#define CRLF_COUNT	2
public:
	Request(std::string query);
	~Request();

private:
	httpStatusCode parse();
	httpStatusCode parseBody();
	httpStatusCode parseRequestLine();
	httpStatusCode parseHeaderFields();


	std::string	_query;

	method		_method;
	std::string	_target;
	std::string	_version;
	std::map<std::string, std::string>	_headerFields;
	std::string	_body;
};


