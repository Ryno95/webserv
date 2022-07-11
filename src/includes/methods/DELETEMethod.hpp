#pragma once

#include <methods/AMethod.hpp>
#include <FileInfo.hpp>

namespace Webserver
{
	class DELETEMethod : public AMethod
		{
			public:
				DELETEMethod(const Request &request, const Host& host);
				~DELETEMethod();

				Response	*process(const FileInfo& uri);
	};
}
