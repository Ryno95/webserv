#pragma once

#include <string>
#include <HttpStatusCode.hpp>

class Response
{
public:
	Response(HttpStatusCode code);
	~Response();

/*
	Server response pattern:

	[version] [statuscode] [status]\r\n
	[header_key]: [header_value]\r\n
	...
	[header_key]: [header_value]\r\n
	\r\n
	body
*/

private:
	std::string body;
	HttpStatusCode statusCode;

	void setBody();
};
