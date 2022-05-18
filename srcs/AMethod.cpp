#include <AMethod.hpp>

#include <fstream>

namespace Webserver
{
	AMethod::AMethod(Request const& request, const HostConfig& config) : _request(request), _config(config)
	{
	}

	void AMethod::writeFile()
	{
	}
}
