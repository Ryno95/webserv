#include <GETMethod.hpp>

GETMethod::GETMethod(Request const& request) : AMethod(request)
{
}

GETMethod::~GETMethod()
{
}

Response GETMethod::process()
{
	setInputFile(_request.getTarget());
	return _response;
}
