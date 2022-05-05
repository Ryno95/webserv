#include <AMethod.hpp>
#include <Request.hpp>
#include <Response.hpp>

class POSTMethod : public AMethod
{
	public:
		POSTMethod(const Request &request);
		~POSTMethod();

		std::string createFileName(const std::string &target);
		void		setPostResponseHeaders();
		Response	*process();

		private:
			std::string _location;
};


