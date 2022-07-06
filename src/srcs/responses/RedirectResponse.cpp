#include <responses/RedirectResponse.hpp>

namespace Webserver
{
	RedirectResponse::RedirectResponse(const std::string& url) : Response(HttpStatusCodes::REDIRECT)
	{
		addHeader(Header::Location, url);
	}

	RedirectResponse::~RedirectResponse()
	{
	}
}
