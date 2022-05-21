#pragma once

#include <AMethod.hpp>

namespace Webserver
{
	class DELETEMethod : public AMethod
		{
			public:
				DELETEMethod(const Request &request, const Host& host);
				~DELETEMethod();

				Response	*process();
	};
}
