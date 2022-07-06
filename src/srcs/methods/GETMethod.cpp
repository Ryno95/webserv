#include <methods/GETMethod.hpp>
#include <responses/Response.hpp>
#include <responses/AutoIndexResponse.hpp>
#include <Exception.hpp>
#include <Utility.hpp>
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
		return TargetInfo(path).entryExists();
	}

	Response* GETMethod::process(const TargetInfo& uri)
	{
		DEBUG("Entering GET method!");
		std::string		target(uri.getTarget());

		if (uri.isDir())
		{
			if (dirHasDefaultIndex(target + _host.getDefaultIndex()))
				target = target + _host.getDefaultIndex();
			else if (_host.isAutoIndexEnabled())
				return new AutoIndexResponse(target);
			else
				throw InvalidRequestException(HttpStatusCodes::NOT_FOUND);
		}

		Response* response = new Response();
		response->addFile(target);
		response->addHeader(Header::Server, _host.getName());
		return response;
	}
}
