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

void Response::addHeaderField(std::pair<std::string, std::string> field)
{
	// 1. add header field to map
}
