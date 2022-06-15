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
			HttpStatusCode 		getStatus() const;
		

		private:
			std::string			getExecutablePath(const std::string &exe);
			std::string 		createQueryString();
			void				executeCgiFile();
			void				executeCommand();
			void				reapChild();

			const std::string 	_cgiExecutable;
			int					_pid;
			int					_pipeFd[2];
			const Request&		_request;
			std::stringstream* 	_cgiStream;
			const Host&			_host;
			HttpStatusCode		_status;
	};
}
