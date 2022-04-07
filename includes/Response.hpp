#pragma once

#include <string>
#include <map>
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
	void addHeaderField(std::string key, std::string value);
	std::string getBytes() const;

private:
	std::string _rawBytesToSend;
	bool _isReadyToSend;
	char* _body;
	std::map<std::string, std::string> _headerFields;
	HttpStatusCode _statusCode;

};
