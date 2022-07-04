#pragma once

#include <string>

#include <responses/Response.hpp>

namespace Webserver
{
	class AutoIndexResponse : public Response
	{
	public:
		AutoIndexResponse(const std::string& targetDir);
		~AutoIndexResponse();
	};
}
