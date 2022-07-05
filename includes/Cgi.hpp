#pragma once

#include <map>

#include <Request.hpp>
#include <Host.hpp>
#include <ITimeoutable.hpp>
#include <IPollable.hpp>
#include <ITickable.hpp>
#include <methods/TargetInfo.hpp>
#include <config/ParseTreeUtility.hpp>
#include <ICommand.hpp>

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
		template<class T>
		class Command : public ICommand
		{
		public:
			Command() : _instance(nullptr), _callback(nullptr) {}

			Command(T* instance, void (T::*function)(const std::string&)) : _instance(instance), _callback(function)
			{
			}

			~Command()
			{
			}

			Command& operator=(const Command& ref)
			{
				_instance = ref._instance;
				_callback = ref._callback;
				return *this;
			}

			void callback(const std::string& args)
			{
				(_instance->*_callback)(args);
			}

		private:
			T* _instance;
			void (T::*_callback)(const std::string&);
		};

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

			std::map<std::string, Command<Cgi> > getKeywords();
			void statusCallback(const std::string& arg);
			void locationCallback(const std::string& arg);
			void contentTypeCallback(const std::string& arg);

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
			void			parseResult();
			void			processHeaderFields(const HeaderFields& headerFields);

			const std::string 					_cgiExecutable;
			int									_pid;
			int									_pipeFd[2];
			const Request&						_request;
			std::stringstream*					_sendStream;
			const Host&							_host;
			HttpStatusCode						_status;
			const TargetInfo&					_uri;
			CgiResponse&						_response;
			std::map<std::string, std::string> 	_env;
			std::string							_buffer;
	};

	
}
