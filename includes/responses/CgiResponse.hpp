#pragma once

#include <Request.hpp>
#include <responses/Response.hpp>


namespace Webserver
{
	class CgiResponse : public Response
	{
		public:
			enum FDs
			{
				READ_FD,
				WRITE_FD
			} FDs;
	
			CgiResponse(const Request &request);
			~CgiResponse();

		void 		performCGI();


		private:
			std::string	getExecutablePath(const std::string &exe);
			
			const Request 		&_request; 
			int					_pid;
			int					_pipeFd[2];
			const std::string 	_cgiExecutable;
			const std::string 	_envExecutable;

	};

}

