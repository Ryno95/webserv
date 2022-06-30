#pragma once

#include <responses/Response.hpp>
#include <config/HostFields.hpp>
#include <Host.hpp>
#include <defines.hpp>

namespace Webserver
{
	class BadResponse: public Response
	{
		public:
			BadResponse(HttpStatusCode code);
			BadResponse(const Host& host, HttpStatusCode code);
			~BadResponse();

		private:
			void addErrorFile(const std::string& defaultError, const HostFields::ErrorPages& errorPages);
	};
}
