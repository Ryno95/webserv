#include <iostream>
#include <CGI.hpp>
#include <unistd.h>

namespace Webserver
{
	CGI::CGI(const Request &request, const Router &router) : _request(request), _router(router)
	{	
	}	

	CGI::~CGI()	{}

	void	CGI::performCGI()
	{
		if (pipe(pipeFd) <  0)
			throw SystemCallFailedException("Pipe");

		pid = fork();
		if (pid < 0)
			throw SystemCallFailedException("Fork");
		else if (pid > 0)
		{
			// parent process
			wait(NULL);
		}
		else
		{
			// chilld process
		}
		
	}
	
}
		