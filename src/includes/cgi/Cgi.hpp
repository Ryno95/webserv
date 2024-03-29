#pragma once

#include <map>

#include <Request.hpp>
#include <Host.hpp>
#include <ITimeoutable.hpp>
#include <IPollable.hpp>
#include <ITickable.hpp>
#include <FileInfo.hpp>
#include <config/ParseTreeUtility.hpp>
#include <ICommand.hpp>
#include <cgi/Pipes.hpp>
#include <cgi/Executable.hpp>

#include <unistd.h>

namespace Webserver
{
	#define SYSTEM_CALL_ERROR -1
	#define CHILD_PROCESS  0

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
	
			Cgi(const Request &request, const Host &host, const FileInfo& uri, CgiResponse& response);
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
			HttpStatusCode 	getStatus() const;

		private:
			std::string		getExecutablePath(const std::string &exe);
			std::string 	createQueryString();
			void			determineExecutable();
			void			redirectIO();
			void			reapChild();

			std::map<std::string, std::string>	createEnv();
			void			parseResult();
			void			processHeaderFields(const HeaderFields& headerFields);
			Executable		getExecutable(const FileInfo& uri) const;
			std::map<std::string, std::string> createExecutablesMap() const;

			int									_pid;
			Pipes								_pipes;
			const Request&						_request;
			std::stringstream*					_sendStream;
			const Host&							_host;
			HttpStatusCode						_status;
			const FileInfo&						_uri;
			CgiResponse&						_response;
			std::string							_buffer;
			bool								_bodyIsSent;
			const std::map<std::string, std::string> _executables;
	};
}
