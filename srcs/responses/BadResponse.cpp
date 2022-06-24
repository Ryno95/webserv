#include <iostream>

#include <responses/BadResponse.hpp>
#include <Utility.hpp>
#include <Logger.hpp>

namespace Webserver
{
	BadResponse::BadResponse(HttpStatusCode code) : Response(code)
	{
		addErrorFile();
	}

	BadResponse::~BadResponse()
	{
	}

	void BadResponse::addErrorFile()
	{
		// search for the http status code in a map containing:
		// code -> file
		// if not found, send general error page
	}
}
