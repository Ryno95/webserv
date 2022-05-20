#include <iostream>
#include <CGI.hpp>
#include <unistd.h>
#include <Exception.hpp>
#include <PollHandler.hpp>
#include <Host.hpp>

namespace Webserver
{
	CGI::CGI(const Request &request) : _request(request)
	{
		// Host cgiHost = Host::determine(_request.getHost());
		if (pipe(pipeFd) <  0)
			throw SystemCallFailedException("Pipe()");
		PollHandler::addPollfd(pipeFd[READ_FD]);
		PollHandler::addPollfd(pipeFd[WRITE_FD]);
	}	

	CGI::~CGI()
	{
		PollHandler::removePollfd(pipeFd[READ_FD]);
		PollHandler::removePollfd(pipeFd[WRITE_FD]);
	}

	void	CGI::performCGI()
	{
		pid = fork();
		if (pid < 0)
			throw SystemCallFailedException("Fork()");
		else if (pid > 0)
		{
			// parent process
			// read here
			close(pipeFd[WRITE_FD]);
			
			wait(NULL);
			
		}
		else
		{
			// 	close pipe reading end reading 
			if (close(pipeFd[READ_FD]) == -1)
				throw SystemCallFailedException("Close()");

			// 	set pipe writing end to stdout
			if (dup2(pipeFd[WRITE_FD], STDOUT_FILENO) < 0)
				throw SystemCallFailedException("Dup2()");
			
			// 	find cgo script to execute
			const std::string cgiScript = "root/cgi-bin/add.py";

			// execve()
			// 	execve that bitch
			//	chilld process
			//	write here
			close(pipeFd[READ_FD]);
			if (dup2(pipeFd[WRITE_FD], STDOUT_FILENO) < 0)
				throw SystemCallFailedException("dup2()");
		}
		
	}
	
}
