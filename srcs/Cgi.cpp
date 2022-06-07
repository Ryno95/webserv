#include <iostream>
#include <Cgi.hpp>
#include <responses/OkStatusResponse.hpp>
#include <unistd.h>
#include <Exception.hpp>
#include <PollHandler.hpp>
#include <TimeoutHandler.hpp>
#include <Host.hpp>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <Logger.hpp>
#include <Utility.hpp>

#include <Sender.hpp>

#define TERMINATOR '\0'

namespace Webserver
{
	// add Root to target, again
	Cgi::Cgi(const Request &request, const Host &host)
		:	_cgiExecutable(getExecutablePath("/python3")),
			_envExecutable(getExecutablePath("/env")),
			_request(request),
			_cgiStream(new std::stringstream()),
			_host(host)
	{
		if (pipe(_pipeFd) <  0)
			throw SystemCallFailedException("Pipe()");
		_pid = fork();
		if (_pid == SYSTEM_CALL_ERROR)
			throw SystemCallFailedException("Fork()");
		else if (_pid == CHILD_PROCESS)
		{
			try {
				executeCgiFile();
			}
			catch (std::exception &e) {
				std::cerr << "CGI EXECUTION FAILED" << std::endl;
				exit(1);
			}
		}
		else
		{
			close(_pipeFd[WRITE_FD]);

			// WNOHANG returns _pid when the _pid process has actually finished
    		while(waitpid(_pid, NULL, WNOHANG) != _pid);
			if (fcntl(_pipeFd[READ_FD], F_SETFL, O_NONBLOCK) == SYSTEM_ERR)
				throw SystemCallFailedException("fcntl");
			PollHandler::get().add(this);
			TimeoutHandler::get().add(this);
		}
	}	

	Cgi::~Cgi()
	{
		PollHandler::get().remove(this);
		TimeoutHandler::get().remove(this);
		close(_pipeFd[READ_FD]);
	}

	static int	is_executable(const char *full_path_executable)
	{
		struct stat	status;

		return(stat(full_path_executable, &status) == F_OK);
	}

	std::string	Cgi::getExecutablePath(const std::string &exe)
	{
		std::string		all_paths(getenv("PATH"));
		size_t			SinglePathLen;
		int 			i = 0;

		if (all_paths.size() < 1)
			return ("");
		SinglePathLen = all_paths.find(COLON);
		while (SinglePathLen != std::string::npos)
		{
			std::string single_path(all_paths.substr(i, SinglePathLen - i));
			if (is_executable((single_path + exe).c_str()))
				return std::string(single_path + exe);
			i = SinglePathLen + 1;
			SinglePathLen = all_paths.find(COLON, i);
		}
		return ("");
	}

	std::string Cgi::createQueryString()
	{
		const std::string queryStringPrefix("QUERY_STRING=");

		return (queryStringPrefix + _request.getBody()).c_str();
	} 

	void Cgi::executeCommand()
	{
		std::string	completeCgiTarget = prependRoot(_host.getRoot(), _request.getTarget());
		std::string	queryString = createQueryString();
		const char* env[] = {queryString.c_str(), NULL};
		const char* argv[] = {"python3", completeCgiTarget.c_str(), NULL};

		if (execve(_cgiExecutable.c_str(), (char *const *)argv, (char *const *)env) == SYSTEM_CALL_ERROR)
			throw SystemCallFailedException("execve()");
	}

	void Cgi::executeCgiFile()
	{
		if (close(_pipeFd[READ_FD]) == SYSTEM_CALL_ERROR)
			throw SystemCallFailedException("Close()");

		if (dup2(_pipeFd[WRITE_FD], STDOUT_FILENO) == SYSTEM_CALL_ERROR)
			throw SystemCallFailedException("Dup2()");
	
		executeCommand();
	}

	// Write end of the pipe is only used in the child process
	int Cgi::getFd() const
	{
		return _pipeFd[READ_FD];
	}
	
	timeval Cgi::getLastCommunicated() const
	{
		struct timeval time;
		if (gettimeofday(&time, NULL) == SYSTEM_CALL_ERROR)
			throw (SystemCallFailedException("gettimeofday()"));
		return (time);
	}

	void Cgi::onRead()
	{
		char 				buffer[BUFFERSIZE];
        int 				readBytes = 0;

		readBytes = read(_pipeFd[READ_FD], buffer, BUFFERSIZE);
		if (readBytes == SYSTEM_ERR || readBytes == 0)
			return ;
		buffer[readBytes] = '\0';
		*_cgiStream << buffer;
	}

	std::stringstream* 	Cgi::getCgiStream() const
	{
		return _cgiStream;
	}
}
