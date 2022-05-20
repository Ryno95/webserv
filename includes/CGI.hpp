#pragma once

#include <Request.hpp>

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
	
			CGI(const Request &request);
			~CGI();

		void performCGI();
		private:
			const Request 	&_request; 
			int				pid;
			int				pipeFd[2];
	};
}

