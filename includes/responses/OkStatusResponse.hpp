#pragma once

#include <responses/Response.hpp>

namespace Webserver
{
	class OkStatusResponse: public Response
	{
		public:
			OkStatusResponse(HttpStatusCode code);
			OkStatusResponse(std::stringstream* strStream, HttpStatusCode code);
			OkStatusResponse(std::ifstream* stream, const std::string &fileName, HttpStatusCode code);
			virtual ~OkStatusResponse();
	};
}
