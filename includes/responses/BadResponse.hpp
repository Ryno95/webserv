#pragma once

#include <responses/Response.hpp>

namespace Webserver
{
	class BadResponse: public Response
	{
		public:
			BadResponse(HttpStatusCode code);
			~BadResponse();

		private:
			void addErrorFile();
	};
}
