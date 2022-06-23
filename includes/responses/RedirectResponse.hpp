#pragma once

#include <string>

#include <responses/AEmptyBodyResponse.hpp>

namespace Webserver
{
	class RedirectResponse : public AEmptyBodyResponse
	{
	public:
		RedirectResponse(const std::string& url);
		~RedirectResponse();
	};
}
