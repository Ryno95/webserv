#include <iostream>

#include <responses/BadResponse.hpp>
#include <Utility.hpp>
#include <Logger.hpp>

namespace Webserver
{
	BadResponse::BadResponse(HttpStatusCode code) : Response(code)
	{
	}

	BadResponse::BadResponse(const Host& host, HttpStatusCode code) : Response(code)
	{
		addErrorFile(host.getDefaultError(), host.getErrorPages());
	}
	

	BadResponse::~BadResponse()
	{
	}

	void BadResponse::addErrorFile(const std::string& defaultError, const HostFields::ErrorPages& errorPages)
	{
		HostFields::ErrorPages::const_iterator errorPage = errorPages.find(_statusCode.first);
		if (errorPage != errorPages.end())
		{
			try
			{
				addFile(ERROR_PAGES_DIR + errorPage->second);
				return;
			}
			catch(const std::exception& e)
			{
			}
		}

		try
		{
			addFile(ERROR_PAGES_DIR + defaultError);
		}
		catch(const std::exception& e)
		{
			ERROR("Default error not found!");
		}
	}
}
