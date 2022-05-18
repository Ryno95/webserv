#include <fstream>
#include <iostream>
#include <ctime>

#include <config/ServerConfig.hpp>
#include <HttpStatusCode.hpp>
#include <POSTMethod.hpp>
#include <Logger.hpp>
#include <responses/OkStatusResponse.hpp>
#include <responses/BadStatusResponse.hpp>
#include <unistd.h>

namespace Webserver
{
POSTMethod::POSTMethod(const Request &request, const HostConfig& config) : AMethod(request, config) {}

	POSTMethod::~POSTMethod() {}

	std::string POSTMethod::createFileName(const std::string &target)
	{
		// const std::string	timestamp_s = std::to_string(time(nullptr));
		const std::string	root(ROOT); // Will be replaced by config file
		std::string			fileName;
		
		fileName =  root;
		// if (target == "/")
		// 	fileName += timestamp_s;
		// else
		// {
			size_t found = target.find_last_of("/");
			fileName += target.substr(1, found);
			fileName += target.substr(found + 1);
		// }
		return (fileName);
	}

//  Location: http://www.w3.org/pub/WWW/People.html
	void POSTMethod::addLocationHeader()
	{
		const std::string HttpProtocol("http://");
		const std::string localHost("127.0.0.1");
		const std::string listenPort("8080");
		const std::string absPath = _absPathForCreatedFile.substr(_absPathForCreatedFile.find_first_of("/"),  _absPathForCreatedFile.size());
		const std::string location(HttpProtocol + localHost + ":" + listenPort + absPath);

		_response->addHeaderField("Location", location);
		_response->addHeaderField("Created-file", _absPathForCreatedFile);
	}

	void POSTMethod::setPostResponseHeaders(bool isCreated)
	{
		_response->addHeaderField("Content-Length", std::to_string(_request.getBodySize()));
		if (isCreated)
			addLocationHeader();

	}

	Response* POSTMethod::process()
	{
		const std::string	fileName  = createFileName(_request.getTarget());
		std::ofstream       *outfile = new std::ofstream();
		bool				isCreatingNewFile = false;
		
		// check if file aready exists
		if (access(fileName.c_str(), R_OK) == -1)
			isCreatingNewFile = true;
		outfile->open(fileName, std::ios_base::app);
		if (!outfile->is_open())
		{
			perror("");
			_response = new BadStatusResponse(HttpStatusCodes::NOT_FOUND);
			delete outfile;
			return _response;
		}
		// Writing process will be in chunks
		_absPathForCreatedFile = fileName;
		*outfile << _request.getBody();
		delete outfile;

		_response = new OkStatusResponse(HttpStatusCodes::OK);
		if (isCreatingNewFile == true)
			_response->setStatusCode(HttpStatusCodes::CREATED);
		setPostResponseHeaders(isCreatingNewFile);

		return _response;
	}
}
