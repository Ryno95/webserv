#include <iostream>
#include <methods/Cgi.hpp>
#include <responses/OkStatusResponse.hpp>
#include <unistd.h>
#include <Exception.hpp>
#include <PollHandler.hpp>
#include <Host.hpp>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>


#define TERMINATOR '\0'

namespace Webserver
{
	Cgi::Cgi(const Request &request, const Host &host)
		: 	_request(request), _cgiExecutable(getExecutablePath("/Python3")),
			_envExecutable(getExecutablePath("/env")),
			_host(host)
	{
		if (pipe(_pipeFd) <  0)
			throw SystemCallFailedException("Pipe()");
		PollHandler::addPollfd(_pipeFd[READ_FD]);
		PollHandler::addPollfd(_pipeFd[WRITE_FD]);
		performCGI();
	}	

	Cgi::~Cgi()
	{
		PollHandler::removePollfd(_pipeFd[READ_FD]);
		PollHandler::removePollfd(_pipeFd[WRITE_FD]);
	}

	static int	is_executable(const char *full_path_executable)
	{
		struct stat	status;

		DEBUG(full_path_executable);
		return(stat(full_path_executable, &status) == F_OK);
	}

	std::string	Cgi::getExecutablePath(const std::string &exe)
	{
		std::string		all_paths(getenv("PATH"));
		size_t			SinglePathLen;
		int 			i = 0;

		if (all_paths.size() < 1)
			return (NULL);
		SinglePathLen = all_paths.find(COLON);
		while (SinglePathLen != std::string::npos)
		{
			std::string single_path(all_paths.substr(i, SinglePathLen - i));
			if (is_executable((single_path + exe).c_str()))
				return single_path + exe;
			i = SinglePathLen + 1;
			SinglePathLen = all_paths.find(COLON, i);
		}
		return (NULL);
	}


	const char *Cgi::createQueryString()
	{
		const std::string queryStringPrefix("QUERY_STRING=");

		return (queryStringPrefix + _request.getBody()).c_str();
	} 

	int Cgi::executeCommand(const char *queryString, const char *cgiPath)
	{
		const char *argv[] = {"env", "-i", queryString, _cgiExecutable.c_str(), cgiPath, NULL};
		if (execve(_envExecutable.c_str(),(char *const *)argv, NULL) == -1)
				ERROR("EXECVE FAILED");
	}

	void	Cgi::performCGI()
	{
		_pid = fork();
		if (_pid < 0)
			throw SystemCallFailedException("Fork()");
		else if (_pid > 0)
		{
			if (close(_pipeFd[WRITE_FD]) == -1)
				throw SystemCallFailedException("close()");
			char buffer[BUFFERSIZE];
        	int readBytes = 0;

			// reading needs to be done via poll somehow
        	while (1)
        	{
        	    readBytes = read(_pipeFd[READ_FD], buffer, BUFFERSIZE);
        	    if (readBytes <=  0)
        	        break ;
        		buffer[readBytes] = '\0';
				_cgiStream << buffer;
        	}
        	std::cout << _cgiStream.str() << std::endl;
			close(_pipeFd[READ_FD]);
        	wait(NULL);
		}
		else
		{
			const char *cgiPath = "root/cgi-bin/add.py"; // will be replaced by host location/route
			const char *queryString = createQueryString();

			if (close(_pipeFd[READ_FD]) == -1)
				throw SystemCallFailedException("Close()");

			if (dup2(_pipeFd[WRITE_FD], STDOUT_FILENO) < 0)
				throw SystemCallFailedException("Dup2()");

			
			if (executeCommand(queryString, cgiPath) == -1)
				throw SystemCallFailedException("execve()");
			close(_pipeFd[READ_FD]);
		}
		_statusCode = HttpStatusCodes::OK;
	}
}
