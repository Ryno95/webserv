#pragma once

#include <Request.hpp>
#include <Host.hpp>
#include <ITimeoutable.hpp>
#include <IPollable.hpp>

namespace Webserver
{
	#define SYSTEM_CALL_ERROR -1
	#define CHILD_PROCESS  0

	class Cgi : public ITimeoutable, public IPollable
	{
		public:
			enum FDs
			{
				READ_FD,
				WRITE_FD
			} FDs;
	
			Cgi(const Request &request, const Host &host);
			~Cgi();

		void		execute();

		void		onRead();
		void		onWrite() {};
		int			getFd() const;

		// what to do on timeout???
		void 	onTimeout() {WARN("ENTIRING TIEOUT CGI");};
		timeval getLastCommunicated() const;
		
		std::stringstream* 	getCgiStream() const;
		

		private:
			std::string			getExecutablePath(const std::string &exe);
			const char* 		createQueryString();
			void				executeCgiFile();
			void				executeCommand(const char *queryString, const char *cgiPath);

			const std::string 	_cgiExecutable;
			const std::string 	_envExecutable;
			int					_pid;
			int					_pipeFd[2];
			const Request&		_request;
			std::stringstream* 	_cgiStream;
			// cgiStream

	};
}

