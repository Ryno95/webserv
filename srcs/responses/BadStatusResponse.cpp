
#include <iostream>
#include <responses/BadStatusResponse.hpp>
#include <Utility.hpp>
#include <Logger.hpp>
#include <config/GlobalConfig.hpp>

namespace Webserver
{
	BadStatusResponse::BadStatusResponse(HttpStatusCode code) : Response(code)
	{
	}

	BadStatusResponse::BadStatusResponse(HttpStatusCode code, const std::string &errorPage) : Response(code)
	{

		setBodyStream(new std::ifstream(errorPage));
		if (!_bodyStream->is_open())
			throw std::runtime_error("Errorfile doens't exist");
		createContentHeaders(errorPage);
	}
	
	BadStatusResponse::~BadStatusResponse()
	{
	}	
}
