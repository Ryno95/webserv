#pragma once

#include <string>

#include <responses/OkStatusResponse.hpp>

namespace Webserver
{
	class AutoIndexResponse : public OkStatusResponse
	{
	public:
		AutoIndexResponse(const std::string& targetDir);
		~AutoIndexResponse();
	};
}
