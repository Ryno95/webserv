#include <GETMethod.hpp>

GETMethod::GETMethod(Request const& request) : AMethod(request)
{
}

GETMethod::~GETMethod()
{
}

std::string createFilePath(std::string path)
{
	path.erase(0, 1);
	return path;
}

Response* GETMethod::process()
{
	std::ifstream* stream = new std::ifstream();

	stream->open(createFilePath(_request.getTarget()));
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
