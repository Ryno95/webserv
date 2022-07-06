#include <responses/AutoIndexResponse.hpp>
#include <logger/Logger.hpp>
#include <autoIndex/AutoIndex.hpp>
#include <headerFields/HeaderFields.hpp>
#include <utils/Utility.hpp>

namespace Webserver
{
	AutoIndexResponse::AutoIndexResponse(const std::string& targetDir) :
		Response(HttpStatusCodes::OK)
	{
		const std::string autoIndexPage = AutoIndex(targetDir).getHtmlPage();
		setBodyStream(new std::stringstream(autoIndexPage));
		addHeader(Header::ContentLength, toString(autoIndexPage.size()));
	}

	AutoIndexResponse::~AutoIndexResponse()
	{
	}
}
