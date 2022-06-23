#pragma once

#include <responses/AFileResponse.hpp>

namespace Webserver
{
	class OkStatusResponse: public AFileResponse
	{
		public:
			OkStatusResponse(HttpStatusCode code);
			OkStatusResponse(std::stringstream* strStream, HttpStatusCode code);
			OkStatusResponse(std::ifstream* stream, const std::string &fileName, HttpStatusCode code);
			~OkStatusResponse();
	};
}
