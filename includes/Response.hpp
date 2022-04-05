#pragma once

#include <string>
#include <HttpStatusCode.hpp>

/*
	Server response pattern:

	[version] [statuscode] [status]\r\n
	[header_key]: [header_value]\r\n
	...
	[header_key]: [header_value]\r\n
	\r\n
	body
*/

class Response
{
public:
	Response();
	Response(HttpStatusCode code);
	~Response();

	void setBody(char* bytes);
	void addHeaderField(std::pair<std::string, std::string> field);

private:
	char* _body;
	bool _isReadyToSend;
	std::string _rawBytesToSend;
	std::map<std::string, std::string> _headerFields;
	HttpStatusCode _statusCode;

};
