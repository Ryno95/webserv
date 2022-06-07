#include <responses/CgiResponse.hpp>
#include <responses/OkStatusResponse.hpp>
#include <HttpStatusCode.hpp>
#include <HeaderFields.hpp>
#include <unistd.h>

namespace Webserver
{
	CgiResponse::CgiResponse(const Request &request, const Host &host) : Response(request.getStatus()), _cgiRequest(Cgi(request, host))
	{
		ERROR("HERE");
		addHeader(Header::ContentType, "text/html");
		_cgiStream = _cgiRequest.getCgiStream();
	}	
}
