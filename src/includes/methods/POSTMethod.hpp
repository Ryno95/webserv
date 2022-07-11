#include <methods/AMethod.hpp>
#include <FileInfo.hpp>
#include <Request.hpp>
#include <responses/Response.hpp>

namespace Webserver
{
	class POSTMethod : public AMethod
	{
		public:
			POSTMethod(const Request &request, const Host& host);
			~POSTMethod();

			Response	*process(const FileInfo& uri);

			private:
				void		setPostResponseHeaders(bool isCreated);
				void		addLocationHeader(const std::string& createdFilePath);
	};
}
