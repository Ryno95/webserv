#include <methods/GETMethod.hpp>
#include <responses/OkStatusResponse.hpp>
#include <responses/BadStatusResponse.hpp>
#include <Utility.hpp>
#include <autoIndex/AutoIndex.hpp>


// Explaining the Nginx directory index file#
// By default, Nginx tries to display a directory listing when a particular URL path ends with a /. For example, if you were to use the path /assets/css/,
// then Nginx would look in the /css/ directory to see if it can find an index.html file (that is if it has permissions).
// If the autoindex module is not active and there is no index.html present, then Nginx will return a 404 Not Found error.

// However, in the event that you do have an index.html file in that directory, then Nginx will use that to generate a directory listing of the assets that live within that directory.
//  For example, as seen in the screenshot below, an index is generated within the /test/ directory which then displays both items that live within /test/.

// To add autoindexing
// location			/ 
// {
//    autoindex true
// }

namespace Webserver
{
	GETMethod::GETMethod(Request const& request, const Host& host) : AMethod(request, host)
	{

	}

	GETMethod::~GETMethod() 
	{

	}


	static bool	doesEndOnSlash(const std::string& target)
	{
		const size_t lastChar = target.size() - 1;
		
		return target[lastChar] == '/';
	}
	// add !isMethodAllowed()
	Response* GETMethod::process(const std::string& uri)
	{
		DEBUG("Entering GET method!");

		std::ifstream*	stream = new std::ifstream();
		std::string		target = prependRoot(_host.getRoot(), _request.getTarget());

		// DEBUG("TARGET: " << _request.getTarget());
		if (doesEndOnSlash(_request.getTarget()))
		{
			// DEBUG("ENTERING AUTOIDEX CHECK");
			// if (_host.getDefaultIndex() == "")
			// {
			// 	DEBUG("Setting default Page");
			// 	target = _host.getDefaultIndex();
			// }
			// else 
			if (_host.isAutoIndexEnabled())
			{
				DEBUG("ENTERING AutoIndex");
				std::stringstream *oss = new std::stringstream();
				*oss << AutoIndex(target).getHtmlPage();
				return new OkStatusResponse(oss, HttpStatusCodes::OK);
				exit(1); // return autoindexObject
			}
			else
				return new BadStatusResponse(HttpStatusCodes::NOT_FOUND, NotFoundErrorPage);
		}
		target = prependRoot(_host.getRoot(), _request.getTarget());
		DEBUG("TARGET: " << target);
		stream->open(target);
		if (stream->fail())
		{
			delete stream;
			return new BadStatusResponse(HttpStatusCodes::NOT_FOUND, NotFoundErrorPage);
		}
		return new OkStatusResponse(stream, target, HttpStatusCodes::OK);
	}
}
