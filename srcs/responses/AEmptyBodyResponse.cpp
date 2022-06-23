#include <responses/AEmptyBodyResponse.hpp>

namespace Webserver
{
	AEmptyBodyResponse::AEmptyBodyResponse() : AResponse()
	{
	}

	AEmptyBodyResponse::AEmptyBodyResponse(HttpStatusCode code) : AResponse(code)
	{
	}

	AEmptyBodyResponse::~AEmptyBodyResponse()
	{
	}

	std::istream* AEmptyBodyResponse::getBodyStream()
	{
		return nullptr;
	}
}
