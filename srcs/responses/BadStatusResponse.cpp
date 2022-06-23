
#include <iostream>
#include <responses/BadStatusResponse.hpp>
#include <Utility.hpp>
#include <Logger.hpp>

namespace Webserver
{
	BadStatusResponse::BadStatusResponse(HttpStatusCode code) : AFileResponse(code)
	{
	}

	BadStatusResponse::BadStatusResponse(HttpStatusCode code, const std::string &errorPage) : AFileResponse(code)
	{
		openFile(errorPage);
		createContentHeaders(errorPage);
	}

	BadStatusResponse::~BadStatusResponse()
	{
	}
}
