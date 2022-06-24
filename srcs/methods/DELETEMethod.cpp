#include <Utility.hpp>
#include <sys/stat.h>

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

	Response* DELETEMethod::process(const std::string& uri)
	{
		DEBUG("DELETE METHOD");

		struct stat 		fileInfo;

		if (stat(uri.c_str(), &fileInfo) == SYSTEM_ERR)
			throw InvalidRequestException(HttpStatusCodes::NOT_FOUND);
		else if ((!(fileInfo.st_mode & S_IWUSR) && !(fileInfo.st_mode & S_IXUSR))) // check that file has write/exec permissions
			throw InvalidRequestException(HttpStatusCodes::FORBIDDEN);
		else if (remove(uri.c_str()) == SYSTEM_ERR)
			throw InvalidRequestException(HttpStatusCodes::INTERNAL_ERROR);

		return new Response(HttpStatusCodes::OK);
	}
}
