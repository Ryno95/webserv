#include <methods/GETMethod.hpp>
#include <responses/Response.hpp>
#include <Exception.hpp>
#include <Utility.hpp>

namespace Webserver
{
	GETMethod::GETMethod(Request const& request, const Host& host) : AMethod(request, host)
	{
	}

	GETMethod::~GETMethod() 
	{
	}

	Response* GETMethod::process(const std::string& uri)
	{
		DEBUG("Entering GET method!");

		std::string		target = _request.getTarget();

		if (target == "/")
			target = _host.getDefaultIndex();

		Response* response = new Response();
		response->addFile(prependRoot(_host.getRoot(), target));
		return response;
	}
}
