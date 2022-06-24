#include <methods/GETMethod.hpp>
#include <responses/OkStatusResponse.hpp>
#include <responses/AutoIndexResponse.hpp>
#include <responses/BadStatusResponse.hpp>
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
		std::ifstream*	stream = new std::ifstream();
		std::string		target(uri);

		if (isDir(target))
		{
			if (dirHasDefaultIndex(target + _host.getDefaultIndex()))
				target = uri + _host.getDefaultIndex();
			else if (_host.isAutoIndexEnabled())
				return new AutoIndexResponse(target);
			else
				return new BadStatusResponse(HttpStatusCodes::NOT_FOUND, NotFoundErrorPage);
		}

		stream->open(target);
		if (stream->fail())
		{
			delete stream;
			return new BadStatusResponse(HttpStatusCodes::NOT_FOUND, NotFoundErrorPage);
		}
		return new OkStatusResponse(stream, target, HttpStatusCodes::OK);
	}
}
