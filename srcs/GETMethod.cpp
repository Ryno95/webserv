#include <GETMethod.hpp>

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
			_response->setStatusCode(HttpStatusCodes::NOT_FOUND);
			return _response;
		}

		_response->setBodyStream(stream);
		_response->setStatusCode(HttpStatusCodes::OK);

		return _response;
	}
}
