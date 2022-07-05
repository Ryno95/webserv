#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <string>

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
		int saveStdin = dup(STDIN_FILENO);
		_pipeFd[READ_FD] = SYSTEM_ERR;
		_pipeFd[WRITE_FD] = SYSTEM_ERR;
		if (pipe(_pipeFd) < 0)
			throw InvalidRequestException(HttpStatusCodes::INTERNAL_ERROR);
		write(_pipeFd[READ_FD], _request.getBody().c_str() , _request.getBodySize());
		lseek(_pipeFd[READ_FD], 0, SEEK_SET);
		_pid = fork();
		if (_pid == SYSTEM_CALL_ERROR)
			throw InvalidRequestException(HttpStatusCodes::INTERNAL_ERROR);
		else if (_pid == CHILD_PROCESS)
		{
			try
			{
				createEnv();
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
		dup2(saveStdin, STDIN_FILENO);
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
		const char* argv[] = {"python3", _uri.getTarget().c_str(), NULL};
		char** env = getCStyleEnv();

		for (int i = 0; env[i]; i++)
			std::cerr << env[i] << std::endl;

		if (execve(_cgiExecutable.c_str(), (char *const *)argv, (char *const *)getCStyleEnv()) == SYSTEM_CALL_ERROR)
			throw SystemCallFailedException("execve()");
	}

	void Cgi::executeCgiFile()
	{
		// lseek(_pipeFd[READ_FD], 0, SEEK_SET);
		dup2(_pipeFd[READ_FD], STDIN_FILENO);
		write(_pipeFd[READ_FD], _request.getBody().c_str() , _request.getBodySize());
		// if (close(_pipeFd[READ_FD]) == SYSTEM_CALL_ERROR)
		// 	throw SystemCallFailedException("Close()");
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
			// std::cerr << env[i] << std::endl;
		}
		env[_env.size()] = NULL;
		return env;
	}

	void Cgi::createEnv()
	{
		const int 	numOfMethods = 3;
		const char* methods[numOfMethods] = {"GET", "POST", "DELETE"};
		
		_env["GATEWAY_INTERFACE"] 	= "CGI/1.1";
		// _env["REQUEST_METHOD"] 		= std::string(methods[_request.getMethod()]); // this fucks up....
		// _env["REQUEST_METHOD"] 		= "POST"; // this fucks up....
		_env["REQUEST_METHOD"] 		= "GET"; 

		_env["QUERY_STRING"]		= _request.getBody();

		//  default val as described in the documentation
		_env["CONTENT_LENGTH"]		= "-1";
		if (_request.getBody().size() > 0)
			_env["CONTENT_LENGTH"]	= std::to_string(_request.getBody().size());

		// check if content type is specified, otherwise set to default val octet-stream
		std::string contentTypeVal("");
		if (!_request.tryGetHeader("content_type", contentTypeVal))
			_env["CONTENT_TYPE"]		=  Webserv::config().getMimeTypes().getMIMEType("");
		else
			_env["CONTENT_TYPE"] = "multipart/form-data";

		_env["HTTP_HOST"]			= "https://" + _request.getHost();

		std::string cookieBuffer("");
		if (_request.tryGetHeader("Cookie", cookieBuffer))
			_env["HTTP_COOKIE"]			= cookieBuffer;

		_env["SCRIPT_NAME"]			= _request.getTarget();

		_env["SERVER_PROTOCOL"]		= HTTPVERSION;
		
		_env["SERVER_NAME"]			= _host.getName();
	}

	void Cgi::addEnvElement(const std::string key, const std::string value)
	{
		_env[key] = value;
	}

}
