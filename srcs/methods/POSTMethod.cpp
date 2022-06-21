#include <fstream>
#include <iostream>
#include <ctime>

#include <config/ServerConfig.hpp>
#include <HttpStatusCode.hpp>
#include <methods/POSTMethod.hpp>
#include <Logger.hpp>
#include <responses/OkStatusResponse.hpp>
#include <responses/BadStatusResponse.hpp>
#include <unistd.h>

namespace Webserver
{
	POSTMethod::POSTMethod(const Request &request, const Host& host) : AMethod(request, host)
	{
	}

	POSTMethod::~POSTMethod() {}

	void POSTMethod::addLocationHeader()
	{
		const std::string HttpProtocol("http://");
		const std::string localHost("127.0.0.1");
		const std::string listenPort("8080");
		const std::string absPath = _absPathForCreatedFile.substr(_absPathForCreatedFile.find_first_of("/"),  _absPathForCreatedFile.size());
		const std::string location(HttpProtocol + localHost + ":" + listenPort + absPath);

		_response->addHeader(Header::Location, location);
		_response->addHeader("Created-file", _absPathForCreatedFile);
	}

	void POSTMethod::setPostResponseHeaders(bool isCreated)
	{
		_response->addHeader(Header::ContentLength, std::to_string(_request.getBodySize()));
		if (isCreated)
			addLocationHeader();

	}

	Response* POSTMethod::process(const std::string& uri)
	{
		DEBUG("Entering POST method!");

		const std::string	fileName  = prependRoot(_host.getRoot(), _request.getTarget());
		std::ofstream       *outfile = new std::ofstream();
		bool				isCreatingNewFile = false;
		
		// check if file aready exists
		if (access(fileName.c_str(), R_OK) == -1)
			isCreatingNewFile = true;
		outfile->open(fileName, std::ios_base::app);
		if (!outfile->is_open())
		{
			perror("File not created");
			_response = new BadStatusResponse(HttpStatusCodes::NOT_FOUND);
			delete outfile;
			return _response;
		}
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
