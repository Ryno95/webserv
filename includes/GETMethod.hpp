#pragma once

#include <AMethod.hpp>

#include <string>

namespace Webserver
{
	class GETMethod : public AMethod
	{
	public:
		GETMethod(Request const& request, const HostConfig& config);
		~GETMethod();

		Response* process();
	};
}
