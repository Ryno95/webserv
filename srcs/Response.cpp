#include <iostream>
#include <Response.hpp>


Response::Response() {}


Response::Response(HttpStatusCode code) : _statusCode(code)
{
}

Response::~Response()
{
}

void Response::setStatusCode(HttpStatusCode code)
{
	this->_statusCode = code;
}

void Response::setBody(std::string bytes)
{
	const int sizeOfBytes = bytes.size();

	_body = bytes;
	_headerFields["Content-Length"] = std::to_string(sizeOfBytes);
}

void Response::setIsReadyToSend(bool isReadyToSend)
{
	this->_isReadyToSend = isReadyToSend;
}

void Response::addHeaderFields()
{
	_headerFields.insert(std::pair<std::string, std::string>("Server", "The best"));
	_headerFields.insert(std::pair<std::string, std::string>("Content-Length", "0"));
	_headerFields.insert(std::pair<std::string, std::string>("Accept-Ranges", "bytes"));
}

std::string Response::getBytes() const
{
	std::map<std::string, std::string>::const_iterator cursor = _headerFields.begin();
	std::map<std::string, std::string>::const_iterator end = _headerFields.end();
	
	std::string buffer = HTTPVERSION;
	buffer += " ";
	buffer += std::to_string(_statusCode.first);
	buffer += " ";
	buffer += _statusCode.second;
	while (cursor != end)
	{
		buffer += "\r\n";
		buffer += cursor->first;
		buffer += ": ";
		buffer += cursor->second;
		++cursor;
	}
	buffer += "\r\n\r\n";
	buffer += _body;

	return buffer;
}

bool Response::getIsReadyToSend() const
{
	return this->_isReadyToSend;
}
