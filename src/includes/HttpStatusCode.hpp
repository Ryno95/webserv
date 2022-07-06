#pragma once

#include <string>

typedef std::pair<int, std::string> HttpStatusCode;

namespace HttpStatusCodes
{
	const HttpStatusCode OK					= HttpStatusCode(200, "OK");
	const HttpStatusCode CREATED			= HttpStatusCode(201, "Created");
	const HttpStatusCode REDIRECT			= HttpStatusCode(301, "Redirect");
	const HttpStatusCode BAD_REQUEST		= HttpStatusCode(400, "Bad Request");
	const HttpStatusCode FORBIDDEN			= HttpStatusCode(403, "Forbidden");
	const HttpStatusCode NOT_FOUND			= HttpStatusCode(404, "Not Found");
	const HttpStatusCode METHOD_NOT_ALLOWED	= HttpStatusCode(405, "Method Not Allowed");
	const HttpStatusCode PAYLOAD_TOO_LARGE	= HttpStatusCode(413, "Payload Too Large");
	const HttpStatusCode URI_TOO_LONG		= HttpStatusCode(414, "Request-URI Too Long");
	const HttpStatusCode TEAPOT				= HttpStatusCode(418, "Teapot");
	const HttpStatusCode INTERNAL_ERROR		= HttpStatusCode(500, "Internal Server Error");
	const HttpStatusCode NOT_IMPLEMENTED	= HttpStatusCode(501, "Not Implemented");
}
