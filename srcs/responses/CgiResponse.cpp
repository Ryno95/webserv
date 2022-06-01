#include <responses/CgiResponse.hpp>
#include <responses/OkStatusResponse.hpp>
#include <HttpStatusCode.hpp>
#include <unistd.h>

namespace Webserver
{
	CgiResponse::CgiResponse(const Request &request, const Host &host) : Response(request.getStatus()), _cgiRequest(Cgi(request, host))
	{
		WARN("ENTERING CGI RESPONSE CONSTRUCTOR");
		_cgiStream = _cgiRequest.getCgiStream();
		_cgiRequest.execute();
		// addHeader(Header::ContentLength, "2000");
		DEBUG(_cgiStream->str());
	}	
}
