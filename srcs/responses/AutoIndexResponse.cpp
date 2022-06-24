#include <responses/AutoIndexResponse.hpp>
#include <Logger.hpp>
#include <autoIndex/AutoIndex.hpp>

namespace Webserver
{
	AutoIndexResponse::AutoIndexResponse(const std::string& targetDir) :
		OkStatusResponse(new std::stringstream(AutoIndex(targetDir).getHtmlPage()), HttpStatusCodes::OK)
	{
	}

	AutoIndexResponse::~AutoIndexResponse()
	{
	}
}
