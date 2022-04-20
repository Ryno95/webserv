#pragma once

#include <string>
#include <map>

#include <defines.hpp>
#include <HttpStatusCode.hpp>
#include <Method.hpp>

class Request
{
#define COLON 			":"
#define CRLF			"\r\n"
#define CRLF_CHAR_COUNT	2
#define TERMINATOR_LEN	4

public:
	Request();
	Request(std::string query);
	~Request();

	HttpStatusCode	getStatus() const;
	void 			throwError(HttpStatusCode code);
	void			parse();
	bool 			hasBodyField() const;
	std::string 	getTarget() const;
	method 			getMethod() const;
	size_t 			getBodySize() const;

	const std::string	&getBody() const;
	void				appendBody(const std::string &body);

private:
	size_t 	parseRequestLine();
	void 	parseHeaderFields(size_t pos);
	void 	addKeyValuePair(const std::string &src, size_t newLinePos);

	size_t parseMethod();
	size_t parseTarget(size_t pos);
	size_t parseVersion(size_t pos);

	std::string	_query;

	method		_method;
	std::string	_target;
	std::string	_version;
	std::map<std::string, std::string>	_headerFields;
	std::string	_body;

	HttpStatusCode _status;
};







