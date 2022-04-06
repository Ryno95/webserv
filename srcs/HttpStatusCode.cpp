#include <HttpStatusCode.hpp>

const HttpStatusCode HttpStatusCodes::OK				= HttpStatusCode(200, "OK");
const HttpStatusCode HttpStatusCodes::BAD_REQUEST		= HttpStatusCode(400, "BAD_REQUEST");
const HttpStatusCode HttpStatusCodes::NOT_FOUND			= HttpStatusCode(404, "NOT_FOUND");
const HttpStatusCode HttpStatusCodes::URI_TOO_LONG		= HttpStatusCode(414, "URI_TOO_LONG");
const HttpStatusCode HttpStatusCodes::TEAPOT			= HttpStatusCode(418, "TEAPOT");
const HttpStatusCode HttpStatusCodes::NOT_IMPLEMENTED	= HttpStatusCode(501, "NOT_IMPLEMENTED");
