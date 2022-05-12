#pragma once

#include <Response.hpp>

namespace Webserver
{
	class OkStatusResponse: public Response
	{
		public:
			OkStatusResponse();
			OkStatusResponse(std::ifstream* stream, const std::string &fileName);
			~OkStatusResponse();

		private:
		   void buildHeader(const std::string &fileName);
	};
}
