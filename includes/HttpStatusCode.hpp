#pragma once

#include <string>

typedef std::pair<int, std::string> HttpStatusCode;

namespace HttpStatusCodes
{
	const HttpStatusCode OK					= HttpStatusCode(200, "OK");
	const HttpStatusCode BAD_REQUEST		= HttpStatusCode(400, "BAD_REQUEST");
	const HttpStatusCode NOT_FOUND			= HttpStatusCode(404, "NOT_FOUND");
	const HttpStatusCode URI_TOO_LONG		= HttpStatusCode(414, "URI_TOO_LONG");
	const HttpStatusCode TEAPOT				= HttpStatusCode(418, "TEAPOT");
	const HttpStatusCode NOT_IMPLEMENTED	= HttpStatusCode(501, "NOT_IMPLEMENTED");
}
