#include <responses/OkStatusResponse.hpp>
#include <Utility.hpp>

namespace Webserver
{
	OkStatusResponse::OkStatusResponse(HttpStatusCode code) : AFileResponse(code)
	{
	}

	OkStatusResponse::OkStatusResponse(std::ifstream* stream, const std::string &fileName, HttpStatusCode code) : AFileResponse(code)
	{
		setFileStream(stream);
		createContentHeaders(fileName);
	}

	OkStatusResponse::~OkStatusResponse()
	{
	}
}
