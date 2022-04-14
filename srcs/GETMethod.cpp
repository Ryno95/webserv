#include <GETMethod.hpp>

GETMethod::GETMethod(Request const& request) : AMethod(request)
{
}

GETMethod::~GETMethod()
{
}

Response* GETMethod::process()
{
	_response->setStatusCode(HttpStatusCodes::OK);
	setInputFile(_request.getTarget());
	return _response;
}
