#include <iostream>
#include <Logger.hpp>
#include <responses/AResponse.hpp>
#include <Utility.hpp>
#include <Exception.hpp>
#include <Webserv.hpp>

namespace Webserver
{
	AResponse::AResponse()
	{
	}

	AResponse::AResponse(HttpStatusCode code) : _statusCode(code)
	{
		addConstantHeaderFields();
	}

	AResponse::AResponse(const AResponse &ref)
	{
		*this =	ref;
	}

	AResponse::~AResponse()
	{
	}

	AResponse& AResponse::operator=(const AResponse &rhs)
	{
		this->_statusCode = rhs._statusCode;
		return (*this);
	}

	void AResponse::setStatusCode(HttpStatusCode code)
	{
		this->_statusCode = code;
	}

	void AResponse::addConstantHeaderFields()
	{
		addHeader(Header::Server, SERVER_NAME);
		addHeader(Header::AcceptRanges, "bytes");
		addHeader(Header::Date, getTimeStamp());
	}

	std::istream *AResponse::getHeaderStream()
	{
		std::map<std::string, std::string>::const_iterator cursor = headersBegin();
		std::map<std::string, std::string>::const_iterator end = headersEnd();
		
		std::string	header = HTTPVERSION;
		header += " ";
		header += std::to_string(_statusCode.first);
		header += " ";
		header += _statusCode.second;
		while (cursor != end)
		{
			header += "\r\n";
			header += cursor->first;
			header += ": ";
			header += cursor->second;
			++cursor;
		}
		header += "\r\n\r\n";
		_headerStream << header;
		return &_headerStream;
	}
}
