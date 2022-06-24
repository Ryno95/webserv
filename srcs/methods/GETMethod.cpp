#include <methods/GETMethod.hpp>
#include <responses/Response.hpp>
#include <responses/AutoIndexResponse.hpp>
#include <Exception.hpp>
#include <Utility.hpp>
#include <sys/stat.h>
#include <autoIndex/AutoIndex.hpp>

namespace Webserver
{
	GETMethod::GETMethod(Request const& request, const Host& host) : AMethod(request, host)
	{
	}

	GETMethod::~GETMethod() 
	{
	}

	static bool	dirHasDefaultIndex(const std::string path)
	{
		struct stat fileInfo;

		return stat(path.c_str(), &fileInfo) == 0; 
	}

	static bool isDir(const std::string& target)
	{
		struct stat fileInfo;
		if (stat(target.c_str(), &fileInfo) == 0 && fileInfo.st_mode & S_IFDIR)
			return true;
		return false;
	}

	Response* GETMethod::process(const std::string& uri)
	{
		DEBUG("Entering GET method!");
		std::string		target(uri);

		if (isDir(target))
		{
			if (dirHasDefaultIndex(target + _host.getDefaultIndex()))
				target = uri + _host.getDefaultIndex();
			else if (_host.isAutoIndexEnabled())
				return new AutoIndexResponse(target);
			else
				throw InvalidRequestException(HttpStatusCodes::NOT_FOUND);
		}

		Response* response = new Response();
		response->addFile(target);
		return response;
	}
}
