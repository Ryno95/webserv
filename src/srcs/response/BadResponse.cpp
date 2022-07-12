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
		if (checkSpecificErrorPages(host) == false)
		{
			if (addErrorFile(host.getErrorPagesRoot() + host.getDefaultError()) == false)
			{
				WARN("There is no error-page defined for '" << code.first << " " << code.second <<
					"' and the default error page '" << host.getErrorPagesRoot() + host.getDefaultError() << "' could not be found.");
			}
		}
	}

	BadResponse::~BadResponse()
	{
	}

	bool BadResponse::checkSpecificErrorPages(const Host& host)
	{
		const HostFields::ErrorPages& errorPages = host.getErrorPages();
		HostFields::ErrorPages::const_iterator errorPage = errorPages.find(_statusCode.first);
		if (errorPage == errorPages.end())
			return false;

		try
		{
			addErrorFile(host.getErrorPagesRoot() + errorPage->second);
			return true;
		}
		catch(const std::exception& e)
		{
			return false;
		}
	}

	bool BadResponse::addErrorFile(const std::string& filePath)
	{
		try
		{
			addFile(filePath);
			return true;
		}
		catch(const std::exception& e)
		{
			return false;
		}
	}
}
