#pragma once

#include <Request.hpp>
#include <Router.hpp>

namespace Webserver
{
	class CGI
	{
		public:
			enum FDs
			{
				READ_FD,
				WRITE_FD
			} FDs;
	
			CGI(const Request &request, const Router &router);
			~CGI();

		void performCGI();
		private:
			const Request 	&_request; 
			const Router	&_router; 
			int				pid;
			int				pipeFd[2];
	};
}

