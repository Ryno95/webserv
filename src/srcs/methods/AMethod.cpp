#include <methods/AMethod.hpp>

#include <fstream>

namespace Webserver
{
	AMethod::AMethod(Request const& request, const Host& host) : _request(request), _host(host)
	{
	}

	void AMethod::writeFile()
	{
	}
}
