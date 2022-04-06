#pragma once

#include <string>

typedef std::pair<int, std::string> HttpStatusCode;

struct HttpStatusCodes
{
	static const HttpStatusCode OK;
	static const HttpStatusCode BAD_REQUEST;
	static const HttpStatusCode NOT_FOUND;
	static const HttpStatusCode URI_TOO_LONG;
	static const HttpStatusCode TEAPOT;
	static const HttpStatusCode NOT_IMPLEMENTED;
};
