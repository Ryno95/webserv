#include <methods/AMethod.hpp>
#include <methods/TargetInfo.hpp>
#include <Request.hpp>
#include <responses/Response.hpp>

namespace Webserver
{
	class POSTMethod : public AMethod
	{
		public:
			POSTMethod(const Request &request, const Host& host);
			~POSTMethod();

			Response	*process(const TargetInfo& uri);

			private:
				std::string _location;
				std::string _relativeCreatedPath;

				void		setPostResponseHeaders(bool isCreated);
				void		addLocationHeader();
	};
}
