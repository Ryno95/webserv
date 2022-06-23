#pragma once

#include <methods/AMethod.hpp>

namespace Webserver
{
	class DELETEMethod : public AMethod
		{
			public:
				DELETEMethod(const Request &request, const Host& host);
				~DELETEMethod();

				AResponse	*process(const std::string& uri);
	};
}
