#include <responses/RedirectResponse.hpp>
#include <Logger.hpp>

namespace Webserver
{
	RedirectResponse::RedirectResponse(const std::string& url) : Response(HttpStatusCodes::REDIRECT)
	{
		addHeaderField("Location", url);
	}

	RedirectResponse::~RedirectResponse()
	{
	}
}
