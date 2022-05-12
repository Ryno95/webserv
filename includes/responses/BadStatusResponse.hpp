#pragma once

#include <Response.hpp>

namespace Webserver
{
	class BadStatusResponse: public Response
	{
		public:
			BadStatusResponse();
			BadStatusResponse(HttpStatusCode code, const std::string &errorPage);
			~BadStatusResponse();

		private:
		   void buildHeader(const std::string &errorPage);
	};
}

