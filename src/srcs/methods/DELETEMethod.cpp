#include <Utility.hpp>
#include <HttpStatusCode.hpp>
#include <Logger.hpp>
#include <methods/DELETEMethod.hpp>
#include <responses/Response.hpp>
#include <Exception.hpp>

namespace Webserver
{
	DELETEMethod::DELETEMethod(const Request &request, const Host& host) : AMethod(request, host)
	{
	}

	DELETEMethod::~DELETEMethod()
	{
	}

	Response* DELETEMethod::process(const FileInfo& uri)
	{
		DEBUG("DELETE METHOD");

		if (!uri.entryExists())
			throw InvalidRequestException(HttpStatusCodes::NOT_FOUND);
		else if (!uri.isWriteable() && !uri.isExecutable())
			throw InvalidRequestException(HttpStatusCodes::FORBIDDEN);
		else if (remove(uri.getFullPath().c_str()) == SYSTEM_ERR)
			throw InvalidRequestException(HttpStatusCodes::INTERNAL_ERROR);

		return new Response(HttpStatusCodes::OK);
	}
}
