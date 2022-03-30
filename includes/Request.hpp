#pragma once

#include <string>
#include <map>

#include <defines.hpp>

class Request
{
#define COLON 			":"
#define CRLF			"\r\n"
#define CRLF_CHAR_COUNT	2
#define TERMINATOR_LEN	4

public:
	Request(std::string query);
	~Request();

	httpStatusCode getStatus() const;
	void throwError(httpStatusCode code);
	void parse();

private:
	void parseBody();
	void parseRequestLine();
	void parseHeaderFields();
	void addKeyValuePair(const std::string &src, size_t newLinePos);


	std::string	_query;

	method		_method;
	std::string	_target;
	std::string	_version;
	std::map<std::string, std::string>	_headerFields;
	std::string	_body;

	httpStatusCode _status;
};


