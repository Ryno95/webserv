#pragma once

#include <Request.hpp>
#include <Host.hpp>
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
	
			CgiResponse(const Request &request, const Host &host);
			~CgiResponse();

		void 		performCGI();


		private:
			std::string	getExecutablePath(const std::string &exe);
			const char *createQueryString();
			int			executeCommand(const char *queryString, const char *cgiPath);
			// implement a createResponse, remove inheritance
			
			const Host 			&_host;
			const Request 		&_request; 
			const std::string 	_cgiExecutable;
			const std::string 	_envExecutable;
			int					_pid;
			int					_pipeFd[2];

	};

}

