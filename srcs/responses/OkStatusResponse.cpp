#include <responses/OkStatusResponse.hpp>
#include <config/GlobalConfig.hpp>
#include <Utility.hpp>

namespace Webserver
{
    OkStatusResponse::OkStatusResponse() : Response(HttpStatusCodes::OK)
    {
        buildHeader();
    }

    OkStatusResponse::OkStatusResponse(std::ifstream* stream) : Response(HttpStatusCodes::OK)
    {
       setBodyStream(stream);
       buildHeader();
    }
    
    OkStatusResponse::~OkStatusResponse()
    {
        
    }
    
    void OkStatusResponse::buildHeader()
    {
        addHeaderField("Server", SERVER_NAME);
        addHeaderField("Range", "bytes");
        addHeaderField("Date", getTimeStamp());
        if (_bodyStream != nullptr)
        {
            getBodyStream()->seekg(0, std::ios_base::end);
		    addHeaderField("Content-Length", std::to_string(_bodyStream->tellg()));
            // std::string mimeType = GlobalConfig::get().mimeTypes()
        // addHeaderField("Content-Type: ", images/jpeg); How to dynamically get content type???
        }
    }
}
