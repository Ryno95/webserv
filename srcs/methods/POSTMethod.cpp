#include <fstream>
#include <iostream>
#include <ctime>
#include <unistd.h>

#include <config/ServerConfig.hpp>
#include <HttpStatusCode.hpp>
#include <methods/POSTMethod.hpp>
#include <Logger.hpp>
#include <responses/Response.hpp>
#include <Exception.hpp>

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
		// _response->addHeader(Header::ContentLength, std::to_string(_request.getBodySize()));
		if (isCreated)
			addLocationHeader();
	}

	Response* POSTMethod::process(const std::string& uri)
	{
		DEBUG("Entering POST method!");

		std::ofstream		outfile;
		bool				isCreatingNewFile = false;
		
		// check if file aready exists
		if (access(uri.c_str(), R_OK) == -1)
			isCreatingNewFile = true;

		outfile.open(uri, std::ios_base::app);
		if (!outfile.is_open())
		{
			WARN("Unexpected: File '" << uri << "' could not be opened, while we just tried to create it.");
			throw InvalidRequestException(HttpStatusCodes::NOT_FOUND);
		}

		_absPathForCreatedFile = uri;
		outfile << _request.getBody();

		_response = new Response(HttpStatusCodes::OK);
		if (isCreatingNewFile == true)
			_response->setStatusCode(HttpStatusCodes::CREATED);
		setPostResponseHeaders(isCreatingNewFile);

		return _response;
	}
}
