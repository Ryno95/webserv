#include <iostream>
#include <CGI.hpp>
#include <unistd.h>

namespace Webserver
{
	CGI::CGI(const Request &request, const Router &router) : _request(request), _router(router)
	{
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
			//	chilld process
			//	write here
			close(pipeFd[READ_FD]);
			if (dup2(pipeFd[WRITE_FD], STDOUT_FILENO) < 0)
				throw SystemCallFailedException("dup2()");
		}
		
	}
	
}
		