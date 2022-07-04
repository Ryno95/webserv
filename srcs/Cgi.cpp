#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

#include <Cgi.hpp>
#include <Exception.hpp>
#include <PollHandler.hpp>
#include <TimeoutHandler.hpp>
#include <TickHandler.hpp>
#include <Host.hpp>
#include <Logger.hpp>
#include <Utility.hpp>
#include <Sender.hpp>
#include <Webserv.hpp>
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
			_response(response),
			_bodySize(0)
	{
		if (!_uri.entryExists())
			throw InvalidRequestException(HttpStatusCodes::NOT_FOUND);
		else if (!_uri.isReadable()) 
			throw InvalidRequestException(HttpStatusCodes::FORBIDDEN);

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
		createEnv();

		if (fcntl(_pipeFd[READ_FD], F_SETFL, O_NONBLOCK) == SYSTEM_ERR)
			throw InvalidRequestException(HttpStatusCodes::INTERNAL_ERROR);

		_sendStream = new std::stringstream;
		_lastCommunicated = TimeoutHandler::get().getTime();
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

		if (WIFEXITED(status) && WEXITSTATUS(status) > 0)
		{
			_response.setStatusCode(HttpStatusCodes::INTERNAL_ERROR);
			_response.setBodyStream(nullptr);
		}
		else if (_bodySize != 0)
			_response.addHeader(Header::ContentLength, toString(_bodySize));
		_response.setIsReadyToSend();
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
			if (TargetInfo(single_path + exe).isExecutable())
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
		return _lastCommunicated;
	}

	void Cgi::onRead()
	{
		_lastCommunicated = TimeoutHandler::get().getTime();

		char buffer[BUFFERSIZE];
		int readBytes = 0;

		readBytes = read(_pipeFd[READ_FD], buffer, BUFFERSIZE);
		if (readBytes == SYSTEM_ERR)
		{
			DEBUG("Cgi read is blocking, continue.");
		}
		else if (readBytes == 0)
			reapChild();
		else
		{
			_bodySize += readBytes;
			buffer[readBytes] = '\0';
			if (_sendStream)
				*_sendStream << buffer;
		}
	}

	void Cgi::onTimeout()
	{
		WARN("CGI TIMEOUT!");
		kill(_pid, SIGINT);
		_response.setBodyStream(nullptr);
		_response.setStatusCode(HttpStatusCodes::INTERNAL_ERROR);
		_bodySize = 0;
	}

	void Cgi::onWrite()
	{
	}

	std::istream* 	Cgi::getCgiStream() const { return _sendStream; }

	HttpStatusCode 		Cgi::getStatus() const { return _status; }
	

	void Cgi::createEnv()
	{
		const int 	numOfMethods = 3;
		const char* methods[numOfMethods] = {"GET", "POST", "DELETE"};
		
		_env["GATEWAY_INTERFACE"] 	= "CGI/1.1";
		_env["REQUEST_METHOD"] 		= methods[_request.getMethod()];

		_env["QUERY_STRING"]		= _request.getBody();

		 // default val as described in the documentation
		_env["CONTENT_LENGTH"]		= -1;
		if (_request.getBody().size() > 0)
			_env["CONTENT_LENGTH"]	= _request.getBody().size();

		// check if content type is specified, otherwise set to default val octet-stream
		std::string contentTypeVal("");
		if (!_request.tryGetHeader("CONTENT_TYPE", contentTypeVal))
			_env["CONTENT_TYPE"]		=  Webserv::config().getMimeTypes().getMIMEType("");
		else
			_env["CONTENT_TYPE"] = contentTypeVal;

		_env["HTTP_HOST"]			= "https://" + _request.getHost();
		_env["HTTP_COOKIE"]			= "https://" + _request.getHost();

		_env["SCRIPT_NAME"]			= _request.getTarget();

		_env["SERVER_PROTOCOL"]		= HTTPVERSION;
		
		_env["SERVER_NAME"]			= _host.getName();
	}

	void Cgi::addEnvElement(const std::string key, const std::string value)
	{
		_env[key] = value;
	}

}
// REQUEST_METHOD
// CONTENT_LENGTH = -1 default
// CONTENT_TYPE = MIMETYPE of body in request
// GATEWAY INTERFACE = CGI/1.1
// HTTP_HOST: Specifies the Internet host and port number of the resource being requested. Required for all HTTP/1.1 requests.
// QUERY_STRING
// SCRIPT_NAME = Returns the part of the URL from the protocol name up to the query string in the first line of the HTTP request.
//  Website: https://localhost:8080/add.py
// 		SERVER_NAME: Webserv
// 		HTTP_HOST: https://localhost:8080
// 		SERVER_PORT: 8080
// SERVER_PROTOCOL: HTTP/1.1
// HTTP_COOKIE: HTTP Cookie String.
