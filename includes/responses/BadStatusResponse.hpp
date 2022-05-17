#pragma once

#include <Response.hpp>

namespace Webserver
{
	class BadStatusResponse: public Response
	{
		public:
			BadStatusResponse(HttpStatusCode code);
			BadStatusResponse(HttpStatusCode code, const std::string &errorPage);
			~BadStatusResponse();

		private:
	};
}

