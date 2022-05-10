#include <GETMethod.hpp>

GETMethod::GETMethod(Request const& request) : AMethod(request)
{
}

GETMethod::~GETMethod()
{
}

std::string createFilePath(std::string path)
{
	if (path.find("root") != std::string::npos)
		return path.substr(1, path.size());
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
		stream->close();
		_response->setStatusCode(HttpStatusCodes::NOT_FOUND);
		stream->open(createFilePath("/error.html"));
		if (!stream->is_open())
			throw std::runtime_error("No error page");
	}
	else
		_response->setStatusCode(HttpStatusCodes::OK);

	_response->setBodyStream(stream);

	return _response;
}
