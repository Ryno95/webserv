#pragma once

#include <methods/AMethod.hpp>

#include <string>

namespace Webserver
{
	class GETMethod : public AMethod
	{
	public:
		GETMethod(Request const& request, const Host& host);
		~GETMethod();

		AResponse* process(const std::string& uri);
	};
}
