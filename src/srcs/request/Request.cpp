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
		return "";
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
