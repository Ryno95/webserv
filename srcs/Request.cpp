#include <iostream>
#include <cctype>
#include <cstdlib>

#include <Request.hpp>
#include <Utility.hpp>
#include <Exception.hpp>
#include <Logger.hpp>

namespace Webserver
{
	Request::Request() : _status(HttpStatusCodes::OK)
	{
	}

	Request::~Request()
	{
	}

	void Request::appendBody(const std::string &body)
	{
		_body += body;
	}

	const std::string &Request::getBody() const
	{
		return (_body);
	}

	// void	Request::parseKeyValuePair(const std::string &src, size_t newLinePos)
	// {
	// 	const size_t 	colonPos = src.find(COLON);
	// 	int				i = 0;

	// 	if (colonPos == std::string::npos || std::isspace(src[colonPos - 1]))
	// 		throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);
		
	// 	std::string key = src.substr(i, colonPos);
	// 	while(std::isspace(src[colonPos + CRLF_CHAR_COUNT + i]))
	// 		++i;
	// 	std::string value = src.substr(colonPos + CRLF_CHAR_COUNT + i, newLinePos);
	// 	addHeader(key, value);
	// }

	// std::string	getTrimmedLine(std::string line)
	// {
	// 	std::string::iterator start = line.begin();
	// 	std::string::iterator end = line.end();

	// 	while (start != end && std::isspace(*start))
	// 		++start;
	// 	--end;
	// 	while (std::distance(start, end) > 0 && std::isspace(*end))
	// 		--end;
	// 	return std::string(start, end + 1);
	// }

	// void Request::parseHeaderFields(const std::string& fields)
	// {
	// 	size_t				next = 0, last = pos;
	// 	std::string			trimmedLine;
	// 	while ((next = _query.find(CRLF, last)) != std::string::npos)
	// 	{
	// 		trimmedLine = getTrimmedLine(_query.substr(last, next - last));
	// 		parseKeyValuePair(trimmedLine, next);
	// 		if (isTerminatorStr(_query.substr(next, TERMINATOR_LEN)))
	// 			break ;
	// 		last = next + CRLF_CHAR_COUNT;
	// 	}
	// }

	// void Request::parse()
	// {
	// 	size_t pos = parseRequestLine();
	// 	parseHeaderFields(_query.substr(pos));
	// 	validate();
	// }

	HttpStatusCode Request::getStatus() const
	{
		return _status;
	}

	/*
		The target of a uri is guarenteed to be at least "/"
	*/
	const std::string& Request::getTarget() const
	{
		return _uri.getResourcePath();
	}

	// RFC: 5.2 The Resource Identified by a Request
	std::string Request::getHost() const
	{
		if (_uri.isAbsolute())
			return _uri.getHost();

		std::string value;
		if (tryGetHeader(Header::Host, value))
			return value;
		WARN("No host could be determined, while this should've been caught during Request::validate()");
		throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);
	}

	Method::method Request::getMethod() const
	{
		return _method;
	}

	size_t Request::getBodySize() const
	{
		std::string length;
		if (tryGetHeader(Header::ContentLength, length))
			return std::atol(length.c_str());
		return 0;
	}

	/*
		Set the status of this request, so it will not be executed and send the corresponding error to the client instead.
	*/
	void	Request::setStatus(HttpStatusCode status)
	{
		_status = status;
	}
}
