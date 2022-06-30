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
#include <responses/CgiResponse.hpp>
#include <methods/TargetInfo.hpp>
#include <HeaderFieldParser.hpp>

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

		if (fcntl(_pipeFd[READ_FD], F_SETFL, O_NONBLOCK) == SYSTEM_ERR)
			throw InvalidRequestException(HttpStatusCodes::INTERNAL_ERROR);

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

	void Cgi::statusCallback(const std::string& arg)
	{
		try
		{
			_response.setStatusCode(parseHttpStatusCode(arg));
		}
		catch(const std::exception& e)
		{
			ERROR("Invalid status code received from cgi: " << arg);
			_response.setStatusCode(HttpStatusCodes::INTERNAL_ERROR);
		}
	}

	void Cgi::locationCallback(const std::string& arg)
	{
		Uri uri(arg);
		if (uri.isAbsolute())
			_response.addHeader(Header::Location, arg);
		else
		{
			if (arg.size() > 0 && arg[0] == '/')
			{
				try
				{
					_response.addFile(arg);
					return;
				}
				catch(const std::exception& e)
				{
					ERROR("Cgi server redirected: File existance/permissions error: " << arg);
				}
			}
			_response.setStatusCode(HttpStatusCodes::INTERNAL_ERROR);
		}
	}

	// void Cgi::contentTypeCallback(const std::string& arg)
	// {
	// }

	std::map<std::string, Cgi::Command<Cgi> > Cgi::getKeywords()
	{
		std::map<std::string, Command<Cgi> > keywords;

		keywords[Header::Status] = Command<Cgi>(this, &Cgi::statusCallback);
		keywords[Header::Location] = Command<Cgi>(this, &Cgi::locationCallback);
		// keywords[Header::ContentType] = new Command<Cgi>(Cgi::contentTypeCallback);
		return keywords;
	}

	void Cgi::processHeaderFields(const HeaderFields& headerFields)
	{
		std::map<std::string, Command<Cgi> > keywords = getKeywords();

		HeaderFields::const_iterator it = headerFields.headersBegin();
		HeaderFields::const_iterator end = headerFields.headersEnd();
		while (it != end)
		{
			if (keywords.find(it->first) != keywords.end())
			{
				// The header is a 'special' one we want to process
				keywords[it->first].callback(it->second);
				keywords.erase(it->first);
			}
			else
			{
				// The header was not one we need to process, so we just add it to the response headers
				_response.addHeader(it->first, it->second);
			}
			it++;
		}
	}

	void Cgi::parseResult()
	{
		size_t bodyPos = _buffer.find("\n\n");
		if (bodyPos != std::string::npos)
		{
			HeaderFields headerFields = HeaderFieldParser().setEndl("\n").parse(_buffer);
			processHeaderFields(headerFields);

			bodyPos += 2;
		}
		else
			bodyPos = 0;

		if (bodyPos < _buffer.size() && _response.getBodyStream() == nullptr)
		{
			_response.addHeader(Header::ContentLength, toString(_buffer.size() - bodyPos));
			_response.setBodyStream(new std::stringstream(_buffer.substr(bodyPos)));
		}
	}

	void Cgi::reapChild()
	{
		int status;
		if (waitpid(_pid, &status, WNOHANG) == 0)
		{
			DEBUG("Child not ready to be reaped.");
			return;
		}

		DEBUG("Child reaped with status: " << WIFEXITED(status) << ", exit status: " << WEXITSTATUS(status));

		if (_response.isReadyToSend())
			return;

		if (WEXITSTATUS(status) > 0 || WIFSIGNALED(status))
			_response.setStatusCode(HttpStatusCodes::INTERNAL_ERROR);
		else
		{
			try
			{
				parseResult();
			}
			catch(const std::exception& e)
			{
				ERROR("During parsing cgi");
				_response.setStatusCode(HttpStatusCodes::INTERNAL_ERROR);
			}
		}
		_response.setReadyToSend(true);
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
			buffer[readBytes] = '\0';
			_buffer += buffer;
		}
	}

	void Cgi::onTimeout()
	{
		WARN("CGI TIMEOUT!");
		kill(_pid, SIGINT);
	}

	void Cgi::onWrite()
	{
	}

	HttpStatusCode 		Cgi::getStatus() const { return _status; }
}
