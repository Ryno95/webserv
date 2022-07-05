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
#include <TickHandler.hpp>
#include <Host.hpp>
#include <Logger.hpp>
#include <Utility.hpp>
#include <Sender.hpp>
#include <Webserv.hpp>
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
			_response(response),
			_buffer(_request.getBody()),
			_bodyIsSent(false)
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
				createEnv();
				executeCgiFile();
			}
			catch (std::exception &e) {
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
	void Cgi::executeCommand()
	{
		const char* argv[] = {"python3", _uri.getTarget().c_str(), NULL};
		char** 		env = getCStyleEnv();

		if (execve(_cgiExecutable.c_str(), (char *const *)argv, (char *const *)env) == SYSTEM_CALL_ERROR)
		{
			perror("");
			throw SystemCallFailedException("execve()");
		}
	}

	void Cgi::executeCgiFile()
	{
		if (dup2(_pipes.serverToCgi[READ_FD], STDIN_FILENO) == SYSTEM_CALL_ERROR)
			throw SystemCallFailedException("Dup2()");
		if (dup2(_pipes.CgiToServer[WRITE_FD], STDOUT_FILENO) == SYSTEM_CALL_ERROR)
			throw SystemCallFailedException("Dup2()");
	
		executeCommand();
	}

	// Write end of the pipe is only used in the child process
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
	

	char** Cgi::getCStyleEnv()
	{
		char **env = new char*[_env.size() + 1];
		std::map<std::string, std::string>::const_iterator it = _env.begin();
		std::map<std::string, std::string>::const_iterator end = _env.end();

		for (int i = 0; it != end; it++, i++)
		{
			std::string keyAndVal(it->first + "=" + it->second);
			env[i] = new char[keyAndVal.size() + 1];
			std::strcpy(env[i], keyAndVal.c_str());
		}
		env[_env.size()] = NULL;
		return env;
	}

	void Cgi::createEnv()
	{
		const int 	numOfMethods = 3;
		const char* methods[numOfMethods] = {"GET", "POST", "DELETE"};
		
		_env["GATEWAY_INTERFACE"] = "CGI/1.1";
		_env["REQUEST_METHOD"] = std::string(methods[_request.getMethod()]); // this fucks up....



		if (_request.getMethod() != Method::POST)
			_env["QUERY_STRING"]		= _request.getBody();

		//  default val as described in the documentation
		_env["CONTENT_LENGTH"] = "-1";
		if (_request.getBody().size() > 0)
			_env["CONTENT_LENGTH"] = std::to_string(_request.getBody().size());

		// check if content type is specified, otherwise set to default val octet-stream
		std::string contentTypeVal("");
		if (_request.tryGetHeader("Content-Type", contentTypeVal))
			_env["CONTENT_TYPE"] = contentTypeVal;
		else
			_env["CONTENT_TYPE"] = "multipart/form-data";

		_env["HTTP_HOST"] = "https://" + _request.getHost();

		std::string cookieBuffer("");
		if (_request.tryGetHeader("Cookie", cookieBuffer))
			_env["HTTP_COOKIE"]	= cookieBuffer;

		_env["SCRIPT_NAME"]	= _request.getTarget();

		_env["SERVER_PROTOCOL"]	= HTTPVERSION;
		
		_env["SERVER_NAME"]	= _host.getName();
	}

}
