#pragma once

#include <Request.hpp>
#include <Host.hpp>
#include <ITimeoutable.hpp>
#include <IPollable.hpp>
#include <ITickable.hpp>
#include <methods/TargetInfo.hpp>

namespace Webserver
{
	#define SYSTEM_CALL_ERROR -1
	#define CHILD_PROCESS  0

	typedef struct tmpFiles
	{
		FILE	*fIn;
		FILE	*fOut;
		int		fdIn;
		int		fdOut;
		int 	saveStdIn;
		int 	saveStdOut;
	}			t_tmpFiles;

	class CgiResponse;

	class Cgi : public ITimeoutable, public IPollable
	{
		public:
			enum FDs
			{
				READ_FD,
				WRITE_FD
			} FDs;
	
			Cgi(const Request &request, const Host &host, const TargetInfo& uri, CgiResponse& response);
			~Cgi();

			void		execute();

			void		onRead();
			void		onWrite();
			int			getFd() const;
			void 		onTimeout();

			timeval getLastCommunicated() const;
		
			std::istream* 	getCgiStream() const;
			HttpStatusCode 		getStatus() const;

		private:
			std::string		getExecutablePath(const std::string &exe);
			std::string 	createQueryString();
			void			executeCgiFile();
			void			executeCommand();
			void			reapChild();

			void			createEnv();
			char**			getCStyleEnv();
			void			addEnvElement(const std::string key, const std::string value);

			const std::string 					_cgiExecutable;
			int									_pid;
			int									_pipeFd[2];
			tmpFiles							_files;
			const Request&						_request;
			std::stringstream*					_sendStream;
			const Host&							_host;
			HttpStatusCode						_status;
			const TargetInfo&					_uri;
			CgiResponse&						_response;
			uint								_bodySize;
			std::map<std::string, std::string> 	_env;
	};

	
}
