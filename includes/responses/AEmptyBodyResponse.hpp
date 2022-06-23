#pragma once

#include <responses/AResponse.hpp>

namespace Webserver
{
	class AEmptyBodyResponse : public AResponse
	{
	public:
		virtual ~AEmptyBodyResponse();
		std::istream* getBodyStream();

	protected:
		AEmptyBodyResponse();
		AEmptyBodyResponse(HttpStatusCode code);
	};
}
