#pragma once

#include <Request.hpp>
#include <Host.hpp>
#include <methods/AMethod.hpp>

namespace Webserver
{
	#define SYSTEM_CALL_ERROR -1
	#define CHILD_PROCESS  0

	class Cgi : public AMethod
	{
		public:
			enum FDs
			{
				READ_FD,
				WRITE_FD
			} FDs;
	
			Cgi(const Request &request, const Host &host);
			~Cgi();

		Response	*process();


		private:
			std::string			getExecutablePath(const std::string &exe);
			const char* 		createQueryString();
			void				executeCgiFile();
			int					executeCommand(const char *queryString, const char *cgiPath);
			std::stringstream* 	getCgiStream();

			const std::string 	_cgiExecutable;
			const std::string 	_envExecutable;
			int					_pid;
			int					_pipeFd[2];
			// cgiStream

	};
}

