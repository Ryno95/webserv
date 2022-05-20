#include <AMethod.hpp>
#include <Request.hpp>
#include <responses/Response.hpp>

namespace Webserver
{
	class POSTMethod : public AMethod
	{
		public:
			POSTMethod(const Request &request, const Host& host);
			~POSTMethod();

			Response	*process();

			private:
				std::string _location;
				std::string _absPathForCreatedFile;

				void		setPostResponseHeaders(bool isCreated);
				void		addLocationHeader();
				std::string createFileName(const std::string &target);
	};
}
