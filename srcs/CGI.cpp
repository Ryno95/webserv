#include <iostream>
#include <CGI.hpp>
#include <unistd.h>
#include <Exception.hpp>
#include <PollHandler.hpp>
#include <Host.hpp>
#include <stdlib.h>

namespace Webserver
{
	CGI::CGI(const Request &request) : _request(request)
	{
		// Host cgiHost = Host::determine(_request.getHost());
		if (pipe(pipeFd) <  0)
			throw SystemCallFailedException("Pipe()");
		PollHandler::addPollfd(pipeFd[READ_FD]);
		PollHandler::addPollfd(pipeFd[WRITE_FD]);
		performCGI();
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
			char buffer[100];
        	int readBytes = 0, bufferRead = 0;

        	while (1)
        	{
        	    readBytes = read(pipeFd[READ_FD], &buffer[bufferRead], 2);
        	    if (readBytes <=  0)
        	        break ;
        	    bufferRead += readBytes;
        	}
        	buffer[bufferRead] = '\0';
        	std::cout <<  buffer << std::endl;
			close(pipeFd[READ_FD]);
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
			
			// 	getExecutablePath() = /usr/local/bin/python3;
			const char *exePath = "/usr/local/bin/python3";

			// 	getEnvPath = /usr/bin/env;
			const char *envPath = "/usr/bin/env";

			// 	getCgiPath()		= root/cgi-bin/add.py;
			const char *cgiPath = "root/cgi-bin/add.py";

			// body = QueryString
			const char *queryString = "QUERY_STRING=val1=6&val2=5";
			// const char * queryString = std::string("QUERY_STRING=" + _request.getBody()).c_str();
			DEBUG(queryString);
			const char *argv[] = {"env", "-i", queryString, exePath, cgiPath, NULL};
			std::cout << "before execve" << std::endl;
			if (execve(envPath,(char *const *)argv, NULL) == -1)
				ERROR("EXECVE FAILED");
			std::cout << "after execve" << std::endl;
			// 	execve that bitch
			//	chilld process
			//	write here
			close(pipeFd[READ_FD]);
			if (dup2(pipeFd[WRITE_FD], STDOUT_FILENO) < 0)
				throw SystemCallFailedException("dup2()");
		}
		
	}
	
}
