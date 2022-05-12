#include <OKResponse.hpp>
#include <Utility.hpp>

namespace Webserver
{
    OKResponse::OKResponse() : Response(HttpStatusCodes::OK)
    {
        buildHeader();
    }

    OKResponse::OKResponse(std::ifstream* stream) : Response(HttpStatusCodes::OK)
    {
       _bodyStream = stream;
       buildHeader();
    }
    
    OKResponse::~OKResponse()
    {
        
    }
    
    void OKResponse::buildHeader()
    {
        // addHeaderField("Content-Type: ", getTimeStamp()); How to dynamically get content type???
        addHeaderField("Server", SERVER_NAME);
        addHeaderField("Range", "bytes");
        addHeaderField("Date", getTimeStamp());
        if (_bodyStream != nullptr)
        {
            getBodyStream()->seekg(0, std::ios_base::end);
		    addHeaderField("Content-Length", std::to_string(_bodyStream->tellg()));
        }
    }
}
