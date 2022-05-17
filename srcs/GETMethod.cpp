#include <GETMethod.hpp>
#include <responses/OkStatusResponse.hpp>
#include <responses/BadStatusResponse.hpp>

namespace Webserver
{
	GETMethod::GETMethod(Request const& request) : AMethod(request)
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
		std::ifstream* stream = new std::ifstream();

		std::string target = _request.getTarget();
		if (target == "/")
			target = "/index.html"; // home returned of redirect naar home

		stream->open(createFilePath(target));
		if (stream->fail())
		{
			delete stream;
			return (new BadStatusResponse(HttpStatusCodes::NOT_FOUND, NotFoundErrorPage));
		}
		return (new OkStatusResponse(stream, target, HttpStatusCodes::OK));
	}
}
