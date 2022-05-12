
#include <iostream>
#include <responses/BadStatusResponse.hpp>
#include <Utility.hpp>
#include <Logger.hpp>
#include <config/GlobalConfig.hpp>

namespace Webserver
{
	BadStatusResponse::BadStatusResponse() : Response()
	{
		buildHeader("");
	}

	BadStatusResponse::BadStatusResponse(HttpStatusCode code, const std::string &errorPage) : Response(code)
	{
		_bodyStream = new std::ifstream(errorPage);
		if (!_bodyStream->is_open())
			throw std::runtime_error("Errorfile doens't exist");
		buildHeader(errorPage);
	}
	
	BadStatusResponse::~BadStatusResponse()
	{
	}
	
	// add safety for finlastfo
	void BadStatusResponse::buildHeader(const std::string &errorPage)
	{
		addHeaderField("Server", SERVER_NAME);
		addHeaderField("Range", "bytes");
		addHeaderField("Date", getTimeStamp());
		if (_bodyStream != nullptr && errorPage.size() > 1)
		{
			getBodyStream()->seekg(0, std::ios_base::end);
			addHeaderField("Content-Length", std::to_string(_bodyStream->tellg()));
			std::string fileExtensionWithoutDot = errorPage.substr(errorPage.find_last_of(".") + 1, errorPage.size());
			std::string mimeType = GlobalConfig::get().mimeTypes.getMIMEType(fileExtensionWithoutDot);
			addHeaderField("Content-Type", mimeType);
		}
	}
}
