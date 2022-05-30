#include <methods/DELETEMethod.hpp>

namespace Webserver
{
	DELETEMethod::DELETEMethod(const Request &request, const Host& host) : AMethod(request, host)
	{
	}

	DELETEMethod::~DELETEMethod()
	{
	}

	Response* DELETEMethod::process()
	{
		WARN("DELETE is not yet implemented!");

		return nullptr;
	}
}
