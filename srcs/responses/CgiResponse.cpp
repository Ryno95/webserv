#include <iostream>
#include <responses/CgiResponse.hpp>
#include <unistd.h>
#include <Exception.hpp>
#include <PollHandler.hpp>
#include <Host.hpp>
#include <stdlib.h>
#include <sys/stat.h>


#define TERMINATOR '\0'

namespace Webserver
{
	CgiResponse::CgiResponse(const Request &request)
		: _request(request), _cgiExecutable(getExecutablePath("/Python3")), _envExecutable(getExecutablePath("/env"))
	{
		// Host cgiHost = Host::determine(_request.getHost());
		if (pipe(_pipeFd) <  0)
			throw SystemCallFailedException("Pipe()");
		PollHandler::addPollfd(_pipeFd[READ_FD]);
		PollHandler::addPollfd(_pipeFd[WRITE_FD]);
		// DEBUG("CREATING ")
		performCGI();
	}	

	CgiResponse::~CgiResponse()
	{
		PollHandler::removePollfd(_pipeFd[READ_FD]);
		PollHandler::removePollfd(_pipeFd[WRITE_FD]);
	}

	int	is_executable(const char *full_path_executable)
	{
		struct stat	status;

		DEBUG(full_path_executable);
		return(stat(full_path_executable, &status) == F_OK);
	}

	std::string	CgiResponse::getExecutablePath(const std::string &exe)
	{
		std::string		all_paths(getenv("PATH"));
		size_t			SinglePathLen;

		if (all_paths.size() < 1)
			return (NULL);
		SinglePathLen = all_paths.find(COLON);
		int i = 0;
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

	void	CgiResponse::performCGI()
	{
		_pid = fork();
		if (_pid < 0)
			throw SystemCallFailedException("Fork()");
		else if (_pid > 0)
		{
			// parent process
			// read here
			close(_pipeFd[WRITE_FD]);
			char buffer[100];
        	int readBytes = 0, bufferRead = 0;

        	while (1)
        	{
        	    readBytes = read(_pipeFd[READ_FD], &buffer[bufferRead], 2);
        	    if (readBytes <=  0)
        	        break ;
        	    bufferRead += readBytes;
        	}
        	buffer[bufferRead] = '\0';
        	std::cout <<  buffer << std::endl;
			close(_pipeFd[READ_FD]);
        	wait(NULL);
		}
		else
		{
			// 	close pipe reading end reading 
			if (close(_pipeFd[READ_FD]) == -1)
				throw SystemCallFailedException("Close()");

			// 	set pipe writing end to stdout
			if (dup2(_pipeFd[WRITE_FD], STDOUT_FILENO) < 0)
				throw SystemCallFailedException("Dup2()");

			const char *cgiPath = "root/cgi-bin/add.py";

			// body = QueryString
			// const char *queryString = "QUERY_STRING=val1=6&val2=5";
			const char * queryString = std::string("QUERY_STRING=" + _request.getBody()).c_str();

			DEBUG(queryString);
			const char *argv[] = {"env", "-i", queryString, _cgiExecutable.c_str(), cgiPath, NULL};
			std::cout << "before execve" << std::endl;
			if (execve(_envExecutable.c_str(),(char *const *)argv, NULL) == -1)
				ERROR("EXECVE FAILED");
			// 	execve that bitch
			//	chilld process
			//	write here
			close(_pipeFd[READ_FD]);
			if (dup2(_pipeFd[WRITE_FD], STDOUT_FILENO) < 0)
				throw SystemCallFailedException("dup2()");
		}
		
	}
	Response* CgiResponse::createCgiResponse()
	{
		return nullptr;	
	}
	
}
