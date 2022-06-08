#include <methods/GETMethod.hpp>
#include <responses/OkStatusResponse.hpp>
#include <responses/BadStatusResponse.hpp>
#include <Utility.hpp>

namespace Webserver
{
	GETMethod::GETMethod(Request const& request, const Host& host) : AMethod(request, host)
	{
	}

	GETMethod::~GETMethod()
	{
	}

	std::string createFilePath(std::string path)
	{
	
		path.insert(0, "root");
		return path;
	}

	Response* GETMethod::process()
	{
		DEBUG("Entering GET method!");

		std::ifstream*	stream = new std::ifstream();
		std::string		target = _request.getTarget();

		if (target == "/")
			target = _host.getDefaultIndex(); // home returned of redirect naar home
		stream->open(prependRoot(_host.getRoot(), target));
		// stream->open(createFilePath(target));
		if (stream->fail())
		{
			delete stream;
			return (new BadStatusResponse(HttpStatusCodes::NOT_FOUND, NotFoundErrorPage));
		}
		return (new OkStatusResponse(stream, target, HttpStatusCodes::OK));
	}
}
