#pragma once

#include <methods/AMethod.hpp>
#include <FileInfo.hpp>

#include <string>

namespace Webserver
{
	class GETMethod : public AMethod
	{
	public:
		GETMethod(Request const& request, const Host& host);
		~GETMethod();

		Response* process(const FileInfo& w);
	};
}
