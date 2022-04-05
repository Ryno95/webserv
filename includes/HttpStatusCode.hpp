#pragma once

#include <string>

typedef std::pair<int, std::string> HttpStatusCode;

#define OK				HttpStatusCode(200, "OK")
#define BAD_REQUEST		HttpStatusCode(400, "BAD_REQUEST")
#define NOT_FOUND		HttpStatusCode(404, "NOT_FOUND")
#define URI_TOO_LONG	HttpStatusCode(414, "URI_TOO_LONG")
#define TEAPOT			HttpStatusCode(418, "TEAPOT")
#define NOT_IMPLEMENTED	HttpStatusCode(501, "NOT_IMPLEMENTED")
