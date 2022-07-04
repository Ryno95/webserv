#pragma once

#include <string>

#include <responses/Response.hpp>

namespace Webserver
{
	class RedirectResponse : public Response
	{
	public:
		RedirectResponse(const std::string& url);
		~RedirectResponse();
	};
}
