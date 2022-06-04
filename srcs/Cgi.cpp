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

#include <Sender.hpp>

#define TERMINATOR '\0'

namespace Webserver
{
	// add Root to target, again
	Cgi::Cgi(const Request &request, const Host &host)
		:	_cgiExecutable(getExecutablePath("/Python3")),
			_envExecutable(getExecutablePath("/env")),
			_request(request),
			_cgiStream(new std::stringstream())
	{
		if (pipe(_pipeFd) <  0)
			throw SystemCallFailedException("Pipe()");
	}	

	Cgi::~Cgi()
	{
		WARN("DESTRUCTING CGI OBJECT");
		PollHandler::get().remove(this);
		TimeoutHandler::get().remove(this);
		if (close(_pipeFd[READ_FD]) == SYSTEM_CALL_ERROR)
			throw SystemCallFailedException("close()");
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
		const std::string queryStringPrefix("QUERY_STRING=val1=4&val2=6");

		return (queryStringPrefix.c_str());
	} 

	void Cgi::executeCommand(const char *queryString, const char *cgiPath)
	{
		// const char *argv[] = {"env", "-i", queryString, _cgiExecutable.c_str(), cgiPath, NULL};
		// std::cerr << "EXECUTING " << _cgiExecutable << std::endl;
		// if (execve(_envExecutable.c_str(), (char *const *)argv, NULL) == SYSTEM_CALL_ERROR)
		// 		throw SystemCallFailedException("execve()");
		// ERROR("CGI SCRIPT: " << cgiPath);
		char *query = "QUERY_STRING=val1=4&val2=6";
		// ERROR("Query_strings: " << query);
		const char *env[] = {query, NULL};
		const char *argv[] = {"python3", cgiPath, NULL};
		if (execve("/usr/local/bin/python3", (char *const *)argv, (char *const *)env) == SYSTEM_CALL_ERROR)
		{
			perror("");
			throw SystemCallFailedException("execve()");
		}
	}

	void Cgi::executeCgiFile()
	{
		const char *cgiPath = std::string("root" + _request.getTarget()).c_str(); // root/cgi-bin/add.py"; // will be replaced by host location/route
		const char *queryString = createQueryString();

		if (close(_pipeFd[READ_FD]) == SYSTEM_CALL_ERROR)
			throw SystemCallFailedException("Close()");

			// int dup2(int oldfd, int newfd);
		if (dup2(_pipeFd[WRITE_FD], STDOUT_FILENO) == SYSTEM_CALL_ERROR)
			throw SystemCallFailedException("Dup2()");

		// if (close(_pipeFd[WRITE_FD]) == SYSTEM_CALL_ERROR)
		// 	throw SystemCallFailedException("Close()");
	
		executeCommand(queryString, cgiPath);
	}

	// Write end is only used in the child process
	int Cgi::getFd() const
	{
		// WARN("SUBSCRIBED FD" << _pipeFd[READ_FD]);
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
		int stat;
		char 				buffer[BUFFERSIZE];
        int 				readBytes = 0;

		WARN("ON_READ CGI TRIGGERD");

		readBytes = read(_pipeFd[READ_FD], buffer, BUFFERSIZE);
		WARN("CGI read: " << readBytes);
		if (readBytes == -1)
			return ;
		else if (readBytes == 0)
		{
			WARN("SETTING FINISHED");
			Sender::IS_FINISHED = true;
			return ;
		}
		else
		{
			buffer[readBytes] = '\0';
			_cgiStream->str(_cgiStream->str() + buffer);
		}
	}

	std::stringstream* 	Cgi::getCgiStream() const
	{
		return _cgiStream;
	}

	void	Cgi::execute()
	{
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
		close(_pipeFd[WRITE_FD]);
        waitpid(_pid, NULL, 0);
		if (fcntl(_pipeFd[READ_FD], F_SETFL, O_NONBLOCK) == SYSTEM_ERR)
			throw SystemCallFailedException("fcntl");
		PollHandler::get().add(this);
		TimeoutHandler::get().add(this);
	}
}
