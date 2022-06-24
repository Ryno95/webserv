#include <unistd.h>

#include <Exception.hpp>
#include <responses/CgiResponse.hpp>
#include <HttpStatusCode.hpp>
#include <HeaderFields.hpp>

namespace Webserver
{
	// the pipes need to be save as well as the cgi stream, so instance of CGI() is saved in the response
	CgiResponse::CgiResponse(const Request &request, const Host &host, const std::string& uri) 
		: Response(request.getStatus()),
		_cgiRequest(Cgi(request, host, uri))
	{
		setStatusCode(_cgiRequest.getStatus());
		if (_statusCode == HttpStatusCodes::NOT_FOUND)
			throw InvalidRequestException(HttpStatusCodes::NOT_FOUND);

		addHeader(Header::ContentType, "text/html");
		setBodyStream(_cgiRequest.getCgiStream());
	}

	CgiResponse::~CgiResponse()
	{
	}
}
