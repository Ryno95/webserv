#include <Response.hpp>

Response::Response() {}

Response::Response(Request &request)
{
	
}

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

void Response::setBody(char* bytes)
{
	// 1. add the body to the object
	// 2. add header field appropriate to the body
}

void Response::setIsReadyToSend(bool isReadyToSend)
{
	this->_isReadyToSend = isReadyToSend;
}

void Response::addHeaderField(std::string key, std::string value)
{
	_headerFields.insert(std::pair<std::string, std::string>(key, value));
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
	}
	buffer += "\r\n\r\n";
	buffer += _body;

	return buffer;
}

bool Response::getIsReadyToSend() const
{
	return this->_isReadyToSend;
}
