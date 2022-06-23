#include <methods/AMethod.hpp>
#include <Request.hpp>
#include <responses/AResponse.hpp>

namespace Webserver
{
	class POSTMethod : public AMethod
	{
		public:
			POSTMethod(const Request &request, const Host& host);
			~POSTMethod();

			AResponse	*process(const std::string& uri);

			private:
				std::string _location;
				std::string _absPathForCreatedFile;

				void		setPostResponseHeaders(bool isCreated);
				void		addLocationHeader();
	};
}
