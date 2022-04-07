#pragma once

#include <string>
#include <map>
#include <HttpStatusCode.hpp>
#include <Request.hpp>

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
	Response(Request &request);
	Response(HttpStatusCode code);
	~Response();

	void	setStatusCode(HttpStatusCode code);
	void	setBody(char* bytes);
	void	setIsReadyToSend(bool isReadyToSend);

	void	addHeaderField(std::string key, std::string value);

	std::string		getBytes();
	bool			getIsReadyToSend() const;
	HttpStatusCode	getStatusCode() const;

private:
	std::string 						_rawBytesToSend;
	bool								_isReadyToSend;
	std::string							_body;
	std::map<std::string, std::string>	_headerFields;
	HttpStatusCode 						_statusCode;

};










