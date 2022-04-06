#include <Response.hpp>

Response::Response() {}

Response::Response(HttpStatusCode code) : _statusCode(code)
{
}

Response::~Response()
{
}

void Response::setBody(char* bytes)
{
	// 1. add the body to the object
	// 2. add header field appropriate to the body
}

void Response::addHeaderField(std::string key, std::string value)
{
	_headerFields.insert(std::pair(key, value));
}

std::string Response::getBytes() const
{
	return "";
}
