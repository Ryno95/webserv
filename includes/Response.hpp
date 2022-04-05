#pragma once

#include <string>
#include <HttpStatusCode.hpp>

class Response
{
public:
	Response(HttpStatusCode code);
	~Response();

/*
	std::string buffer = "HTTP/1.1 ";
	buffer += std::to_string(_request.getStatus());
	buffer += "\r\ncontent-length: 17\r\n\r\nSERVER GOES BRRRR";

	Server response pattern:

	version statuscode status\r\n
	header_keys: header_values\r\n
	\r\n
	body
*/

private:
	std::string body;
	HttpStatusCode statusCode;

	void setBody();
};
