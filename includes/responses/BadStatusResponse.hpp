#pragma once

#include <Response.hpp>

namespace Webserver
{
	class BadStatusResponse: public Response
	{
		#define NotFoundErrorPage "NotFoundErrorPage.html"

		public:
			BadStatusResponse();
			BadStatusResponse(HttpStatusCode code);
			~BadStatusResponse();

		private:
		   void buildHeader();
	};
}

