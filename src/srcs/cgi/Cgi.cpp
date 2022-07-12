#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <string>

#include <cgi/Cgi.hpp>
#include <cgi/Pipes.hpp>
#include <Exception.hpp>
#include <PollHandler.hpp>
#include <TimeoutHandler.hpp>
#include <Host.hpp>
#include <Logger.hpp>
#include <Utility.hpp>
#include <responses/CgiResponse.hpp>
#include <FileInfo.hpp>
#include <HeaderFieldParser.hpp>
#include <cgi/Executable.hpp>

#define TERMINATOR '\0'

namespace Webserver
{
	Cgi::Cgi(const Request &request, const Host &host, const FileInfo& uri, CgiResponse& response)
		:	_request(request),
			_host(host),
			_status(HttpStatusCodes::OK),
			_uri(uri),
			_response(response),
			_buffer(_request.getBody()),
			_bodyIsSent(false),
			_executables(createExecutablesMap())
	{
		if (!_uri.entryExists())
			throw InvalidRequestException(HttpStatusCodes::NOT_FOUND);
		else if (!_uri.isReadable()) 
			throw InvalidRequestException(HttpStatusCodes::FORBIDDEN);

		_pipes.openPipes();
		_pid = fork();
		if (_pid == SYSTEM_CALL_ERROR)
			throw InvalidRequestException(HttpStatusCodes::INTERNAL_ERROR);
		else if (_pid == CHILD_PROCESS)
		{
			try
			{
				_pipes.closeForChild();
				redirectIO();
				getExecutable(_uri).setEnv(createEnv()).execute();
			}
			catch (std::exception &e)
			{
				std::cerr << "CGI EXECUTION FAILED" << std::endl;
				exit(1);
			}
		}

		// parent
		_pipes.closeForParent();

		if (fcntl(_pipes.CgiToServer[READ_FD], F_SETFL, O_NONBLOCK) == SYSTEM_ERR)
			throw InvalidRequestException(HttpStatusCodes::INTERNAL_ERROR);
		if (fcntl(_pipes.serverToCgi[WRITE_FD], F_SETFL, O_NONBLOCK) == SYSTEM_ERR)
			throw InvalidRequestException(HttpStatusCodes::INTERNAL_ERROR);
		_lastCommunicated = TimeoutHandler::get().getTime();
		PollHandler::get().add(this);
		PollHandler::get().setWriteEnabled(this, true);
		TimeoutHandler::get().add(this);
	}

	Cgi::~Cgi()
	{
		PollHandler::get().remove(this);
		TimeoutHandler::get().remove(this);
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

	std::map<std::string, Cgi::Command<Cgi> > Cgi::getKeywords()
	{
		std::map<std::string, Command<Cgi> > keywords;

		keywords[Header::Status] = Command<Cgi>(this, &Cgi::statusCallback);
		keywords[Header::Location] = Command<Cgi>(this, &Cgi::locationCallback);
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
			return;

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
				_response.setStatusCode(HttpStatusCodes::INTERNAL_ERROR);
			}
		}
		_response.setReadyToSend(true);
	}

	std::map<std::string, std::string> Cgi::createExecutablesMap() const
	{
		std::map<std::string, std::string> map;

		map[".pl"] = "perl";
		map[".php"] = "php";
		map[".py"] = "python3";
		map[".sh"] = "bash";

		return map;
	}

	Executable Cgi::getExecutable(const FileInfo& fileInfo) const
	{
		size_t pos = fileInfo.getFileName().find_last_of(".");
		if (pos == std::string::npos)
			return Executable(fileInfo.getFullPath());

		std::map<std::string, std::string>::const_iterator it = _executables.find(fileInfo.getFileName().substr(pos));
		if (it == _executables.end())
			return Executable(fileInfo.getFullPath());
		return Executable(fileInfo.getFullPath(), it->second);
	}

	void Cgi::redirectIO()
	{
		if (dup2(_pipes.serverToCgi[READ_FD], STDIN_FILENO) == SYSTEM_CALL_ERROR)
			throw SystemCallFailedException("Dup2()");
		if (dup2(_pipes.CgiToServer[WRITE_FD], STDOUT_FILENO) == SYSTEM_CALL_ERROR)
			throw SystemCallFailedException("Dup2()");	
	}

	/*
		If the body is not yet fully sent to the Cgi process, then we keep on polling for our write fd.
		When writing is finished, we want to receive the result from the Cgi process,
		so we start polling for read the fd.
	*/
	int Cgi::getFd() const
	{
		if (_bodyIsSent)
			return _pipes.CgiToServer[READ_FD];
		return _pipes.serverToCgi[WRITE_FD];
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

		readBytes = read(_pipes.CgiToServer[READ_FD], buffer, BUFFERSIZE);
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
		int written = write(_pipes.serverToCgi[WRITE_FD], _buffer.c_str(), std::min((size_t)BUFFERSIZE, _buffer.size()));
		
		if (written == -1)
			return ;

		_lastCommunicated = TimeoutHandler::get().getTime();
		_buffer.erase(0, written);
		if (written == 0 || _buffer.size() == 0)
		{
			PollHandler::get().remove(this);
			_pipes.tryClose(_pipes.serverToCgi[WRITE_FD]);

			_bodyIsSent = true;
			PollHandler::get().add(this);
		}
	}

	HttpStatusCode 		Cgi::getStatus() const { return _status; }

	std::map<std::string, std::string> Cgi::createEnv()
	{
		std::map<std::string, std::string> env;
		const char* methods[] = {"GET", "POST", "DELETE"};
		
		env["GATEWAY_INTERFACE"] = "CGI/1.1";
		env["REQUEST_METHOD"] = std::string(methods[_request.getMethod()]);

		if (_request.getMethod() != Method::POST)
			env["QUERY_STRING"]		= _request.getBody();

		//  default val as described in the documentation
		env["CONTENT_LENGTH"] = "-1";
		if (_request.getBody().size() > 0)
			env["CONTENT_LENGTH"] = std::to_string(_request.getBody().size());

		// check if content type is specified, otherwise set to default val octet-stream
		std::string contentTypeVal("");
		if (_request.tryGetHeader(Header::ContentType, contentTypeVal))
			env["CONTENT_TYPE"] = contentTypeVal;

		env["HTTP_HOST"] = "http://" + _request.getHost();

		std::string cookieBuffer("");
		if (_request.tryGetHeader("Cookie", cookieBuffer))
			env["HTTP_COOKIE"]	= cookieBuffer;

		env["SCRIPT_NAME"]	= _request.getTarget();

		env["SERVER_PROTOCOL"]	= HTTPVERSION;
		
		env["SERVER_NAME"]	= _host.getName();
		return env;
	}
}
