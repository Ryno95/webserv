#pragma once

#include <methods/AMethod.hpp>
#include <methods/TargetInfo.hpp>

#include <string>

namespace Webserver
{
	class GETMethod : public AMethod
	{
	public:
		GETMethod(Request const& request, const Host& host);
		~GETMethod();

		Response* process(const TargetInfo& w);
	};
}
