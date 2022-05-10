#include <AMethod.hpp>

#include <fstream>

namespace Webserver
{
	AMethod::AMethod(Request const& request) : _request(request), _response(new Response())
	{
	}

	void AMethod::writeFile()
	{
	}
}
