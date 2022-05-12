#pragma once

#include <Response.hpp>

namespace Webserver
{
	class OkStatusResponse: public Response
	{
		public:
			OkStatusResponse();
			OkStatusResponse(std::ifstream* stream);
			~OkStatusResponse();

		private:
		   void buildHeader();
	};
}
