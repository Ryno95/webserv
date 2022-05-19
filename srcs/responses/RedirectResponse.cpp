#include <responses/RedirectResponse.hpp>

namespace Webserver
{
	RedirectResponse::RedirectResponse(const std::string& url)
	{
		// set status code 301
		// add header field Location: [url]
	}

	RedirectResponse::~RedirectResponse()
	{
	}
}
