#include <methods/GETMethod.hpp>
#include <responses/OkStatusResponse.hpp>
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


	static bool	endsOnSlash(const std::string& target)
	{
		const size_t lastChar = target.size() - 1;
		
		return target[lastChar] == '/';
	}

	static bool	dirHasDefaultIndex(const std::string path)
	{
		struct stat fileInfo;

  		return stat(path.c_str(), &fileInfo) == 0; 
	}

	Response* GETMethod::process(const std::string& uri)
	{
		DEBUG("Entering GET method!");

		std::ifstream*	stream = new std::ifstream();
		std::string		target(uri);

		if (endsOnSlash(target))
		{
			if (_host.getDefaultIndex() != "" && dirHasDefaultIndex(target + _host.getDefaultIndex()))
				target = uri + _host.getDefaultIndex();
			else if (_host.isAutoIndexEnabled())
			{
				std::stringstream *oss = new std::stringstream();
				*oss << AutoIndex(target).getHtmlPage();
				return new OkStatusResponse(oss, HttpStatusCodes::OK);
			}
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
