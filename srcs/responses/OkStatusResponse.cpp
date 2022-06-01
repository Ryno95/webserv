#include <responses/OkStatusResponse.hpp>
#include <config/GlobalConfig.hpp>
#include <Utility.hpp>

namespace Webserver
{
    OkStatusResponse::OkStatusResponse(HttpStatusCode code) : Response(code)
    {
    }

    OkStatusResponse::OkStatusResponse(std::ifstream* stream, const std::string &fileName, HttpStatusCode code) : Response(code)
    {
       setBodyStream(stream);
       createContentHeaders(fileName);
    }

    OkStatusResponse::OkStatusResponse(std::stringstream* strStream, HttpStatusCode code) : Response(code)
    {
        _cgiStream = strStream;
    }
    
    OkStatusResponse::~OkStatusResponse()
    {
        
    }
}
