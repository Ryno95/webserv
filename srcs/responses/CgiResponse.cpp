#include <responses/CgiResponse.hpp>
#include <responses/OkStatusResponse.hpp>
#include <HttpStatusCode.hpp>
#include <HeaderFields.hpp>
#include <unistd.h>

namespace Webserver
{
	// the pipes need to be save as well as the cgi stream, so instance of CGI() is saved in the response
	CgiResponse::CgiResponse(const Request &request, const Host &host) 
		: Response(request.getStatus()), _cgiRequest(Cgi(request, host))
	{
		this->setStatusCode(_cgiRequest.getStatus());
		if (_statusCode == HttpStatusCodes::NOT_FOUND)
		{
			setBodyStream(new std::ifstream("root/NotFoundErrorPage.html"));
			if (!_bodyStream->is_open())
				throw std::runtime_error("Errorfile doens't exist");
			createContentHeaders("root/NotFoundErrorPage.html");
		}
		addHeader(Header::ContentType, "text/html");
		_cgiStream = _cgiRequest.getCgiStream();
	}	
}
