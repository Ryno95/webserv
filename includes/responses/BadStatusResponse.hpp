#pragma once

#include <responses/AFileResponse.hpp>

namespace Webserver
{
	class BadStatusResponse: public AFileResponse
	{
		public:
			BadStatusResponse(HttpStatusCode code);
			BadStatusResponse(HttpStatusCode code, const std::string &errorPage);
			~BadStatusResponse();
	};
}
