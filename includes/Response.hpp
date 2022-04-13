#pragma once

#include <fstream>
#include <map>
#include <string>
#include <sstream>

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
	Response(const Response &ref);
	Response(HttpStatusCode code);
	~Response();

	void	setStatusCode(HttpStatusCode code);
	void	setBody(std::string bytes);

	Response			&operator=(const Response &rhs);

	std::stringstream	*getHeaderStream();
	void				addHeaderFields(); // hard coded for now to get the flow going

	std::fstream		*getBodyStream();
	HttpStatusCode		getStatusCode() const;

private:

	std::stringstream 					_headerStream;
	std::fstream						_bodyStream;
	std::map<std::string, std::string>	_headerFields;
	HttpStatusCode 						_statusCode;
};
