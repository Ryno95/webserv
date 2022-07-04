#pragma once

#include <methods/AMethod.hpp>
#include <methods/TargetInfo.hpp>

namespace Webserver
{
	class DELETEMethod : public AMethod
		{
			public:
				DELETEMethod(const Request &request, const Host& host);
				~DELETEMethod();

				Response	*process(const TargetInfo& uri);
	};
}
