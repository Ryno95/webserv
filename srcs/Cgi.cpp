#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

#include <Cgi.hpp>
#include <Exception.hpp>
#include <PollHandler.hpp>
#include <TimeoutHandler.hpp>
#include <TickHandler.hpp>
#include <Host.hpp>
#include <Logger.hpp>
#include <Utility.hpp>
#include <Sender.hpp>
#include <responses/CgiResponse.hpp>
#include <methods/TargetInfo.hpp>

#define TERMINATOR '\0'

namespace Webserver
{
	Cgi::Cgi(const Request &request, const Host &host, const TargetInfo& uri, CgiResponse& response)
		:	_cgiExecutable(getExecutablePath("/python3")),
			_request(request),
			_host(host),
			_status(HttpStatusCodes::OK),
			_uri(uri),
			_response(response)
	{
		if (!_uri.entryExists())
			throw InvalidRequestException(HttpStatusCodes::NOT_FOUND);
		// else if (!_uri.isReadable()) // Needs to be implemented yet!
			// throw InvalidRequestException(HttpStatusCodes::FORBIDDEN);

		_pipeFd[READ_FD] = SYSTEM_ERR;
		_pipeFd[WRITE_FD] = SYSTEM_ERR;
		if (pipe(_pipeFd) < 0)
			throw InvalidRequestException(HttpStatusCodes::INTERNAL_ERROR);

		_pid = fork();
		if (_pid == SYSTEM_CALL_ERROR)
			throw InvalidRequestException(HttpStatusCodes::INTERNAL_ERROR);
		else if (_pid == CHILD_PROCESS)
		{
			try
			{
				executeCgiFile();
			}
			catch (std::exception &e) {
				std::cerr << "CGI EXECUTION FAILED" << std::endl;
				exit(1);
			}
		}

		// parent
		close(_pipeFd[WRITE_FD]);
		_pipeFd[WRITE_FD] = SYSTEM_ERR;

		if (fcntl(_pipeFd[READ_FD], F_SETFL, O_NONBLOCK) == SYSTEM_ERR)
			throw InvalidRequestException(HttpStatusCodes::INTERNAL_ERROR);

		_sendStream = new SendStream(new std::stringstream);
		PollHandler::get().add(this);
		TimeoutHandler::get().add(this);
	}	

	Cgi::~Cgi()
	{
		PollHandler::get().remove(this);
		TimeoutHandler::get().remove(this);

		if (_pipeFd[READ_FD] != SYSTEM_ERR)
			close(_pipeFd[READ_FD]);
		if (_pipeFd[WRITE_FD] != SYSTEM_ERR)
			close(_pipeFd[WRITE_FD]);
	}

	void Cgi::reapChild()
	{
		int status;
		if (waitpid(_pid, &status, WNOHANG) == 0)
		{
			DEBUG("Child not ready to be reaped.");
			return;
		}

		DEBUG("Child reaped!");

		if (WIFEXITED(status) && WEXITSTATUS(status) > 0)
		{
			_response.setStatusCode(HttpStatusCodes::INTERNAL_ERROR);
			_response.setBodyStream(nullptr);
		}
		_response.setFinished();
	}

	static int	is_executable(const char *full_path_executable)
	{
		struct stat	status;

		return stat(full_path_executable, &status) == F_OK;
	}

	std::string	Cgi::getExecutablePath(const std::string &exe)
	{
		std::string		all_paths(getenv("PATH"));
		size_t			SinglePathLen;
		int 			i = 0;

		if (all_paths.size() < 1)
			return "";
		SinglePathLen = all_paths.find(COLON);
		while (SinglePathLen != std::string::npos)
		{
			std::string single_path(all_paths.substr(i, SinglePathLen - i));
			if (is_executable((single_path + exe).c_str()))
				return std::string(single_path + exe);
			i = SinglePathLen + 1;
			SinglePathLen = all_paths.find(COLON, i);
		}
		return "";
	}

	std::string Cgi::createQueryString()
	{
		const std::string queryStringPrefix("QUERY_STRING=");

		return (queryStringPrefix + _request.getBody()).c_str();
	} 

	void Cgi::executeCommand()
	{
		std::string	queryString = createQueryString();
		const char* env[] = {queryString.c_str(), NULL};
		const char* argv[] = {"python3", _uri.getTarget().c_str(), NULL};

		// if (access(_uri.c_str(), F_OK ) == SYSTEM_ERR || _cgiExecutable == "")
		// 	exit(2);
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
			throw (SystemCallFailedException("gettimeofday()")); // Server will break
		return time;
	}

	void Cgi::onRead()
	{
		char buffer[BUFFERSIZE];
		int readBytes = 0;

		readBytes = read(_pipeFd[READ_FD], buffer, BUFFERSIZE);
		if (readBytes == SYSTEM_ERR)
		{
			DEBUG("Cgi read is blocking, continue.");
		}
		else if (readBytes == 0)
		{
			_sendStream->setIsFilled();
			reapChild();
		}
		else
		{
			buffer[readBytes] = '\0';
			if (_sendStream)
				*_sendStream << buffer;
		}
	}

	void Cgi::onTimeout()
	{
		WARN("Timeout handling on CGI is not implemented yet!");
	}

	void Cgi::onWrite()
	{
	}

	SendStream* 	Cgi::getCgiStream() const { return _sendStream; }

	HttpStatusCode 		Cgi::getStatus() const { return _status; }
}
