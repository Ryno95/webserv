#include <responses/AutoIndexResponse.hpp>
#include <Logger.hpp>
#include <autoIndex/AutoIndex.hpp>

namespace Webserver
{
	AutoIndexResponse::AutoIndexResponse(const std::string& targetDir) :
		Response(HttpStatusCodes::OK)
	{
		const std::string autoIndexPage = AutoIndex(targetDir).getHtmlPage();
		setBodyStream(new std::stringstream(autoIndexPage));
	}

	AutoIndexResponse::~AutoIndexResponse()
	{
	}
}
