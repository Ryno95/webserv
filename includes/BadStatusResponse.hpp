#pragma once

#include <Response.hpp>

namespace Webserver
{
	class BadStatusResponse: public Response
	{
		public:
			BadStatusResponse();
			BadStatusResponse(HttpStatusCode code);
			~BadStatusResponse();

		private:
		   void buildHeader();
	};
}

