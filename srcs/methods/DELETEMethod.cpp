#include <Utility.hpp>
#include <sys/stat.h>

#include <HttpStatusCode.hpp>
#include <Logger.hpp>
#include <methods/DELETEMethod.hpp>
#include <responses/BadStatusResponse.hpp>
#include <responses/OkStatusResponse.hpp>

namespace Webserver
{
	DELETEMethod::DELETEMethod(const Request &request, const Host& host) : AMethod(request, host)
	{
	}

	DELETEMethod::~DELETEMethod()
	{
	}

	Response* DELETEMethod::process()
	{
		const std::string 	fullTarget = prependRoot(_host.getRoot(), _request.getTarget());
		// const bool			isMethodAllowed = _host.isMethodAccepted(this->_request.getMethod());
		const bool			isMethodAllowed = true;
		struct stat 		fileInfo;

		if (stat(fullTarget.c_str(), &fileInfo) == SYSTEM_ERR)
			return new BadStatusResponse(HttpStatusCodes::NOT_FOUND);
		else if (!isMethodAllowed || (!(fileInfo.st_mode & S_IWUSR) && !(fileInfo.st_mode & S_IXUSR))) // check that file has write/exec permissions
			return new BadStatusResponse(HttpStatusCodes::FORBIDDEN);
		else if (remove(fullTarget.c_str()) == SYSTEM_ERR)
			return new BadStatusResponse(HttpStatusCodes::INTERNAL_ERROR);

		return new OkStatusResponse(HttpStatusCodes::OK);
	}
}
