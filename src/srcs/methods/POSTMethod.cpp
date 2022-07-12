#include <fstream>
#include <iostream>

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

	void POSTMethod::addLocationHeader(const std::string& createdFilePath)
	{
		_response->addHeader(Header::Location, "http://" + _request.getHost() + "/" + createdFilePath);
		_response->addHeader("Created-file", createdFilePath);
	}

	Response* POSTMethod::process(const FileInfo& uri)
	{
		DEBUG("Entering POST method!");

		if (!_host.isUploadAllowed())
			throw InvalidRequestException(HttpStatusCodes::FORBIDDEN);

		std::ofstream		outfile;
		bool				isCreatingNewFile = false;
		
		if (!uri.entryExists())
			isCreatingNewFile = true;

		outfile.open(uri.getFullPath(), std::ios_base::app);
		if (!outfile.is_open())
		{
			WARN("Unexpected: File '" << uri.getFullPath() << "' could not be opened, while we just tried to create it.");
			throw InvalidRequestException(HttpStatusCodes::NOT_FOUND);
		}

		outfile << _request.getBody();

		_response = new Response(HttpStatusCodes::OK);
		if (isCreatingNewFile == true)
		{
			_response->setStatusCode(HttpStatusCodes::CREATED);
			addLocationHeader(uri.getFullPath());
		}
		return _response;
	}
}
