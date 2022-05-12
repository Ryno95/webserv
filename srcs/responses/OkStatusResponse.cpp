#include <responses/OkStatusResponse.hpp>
#include <config/GlobalConfig.hpp>
#include <Utility.hpp>

namespace Webserver
{
    OkStatusResponse::OkStatusResponse() : Response(HttpStatusCodes::OK)
    {
        buildHeader("");
    }

    OkStatusResponse::OkStatusResponse(std::ifstream* stream, const std::string &fileName) : Response(HttpStatusCodes::OK)
    {
       setBodyStream(stream);
       buildHeader(fileName);
    }
    
    OkStatusResponse::~OkStatusResponse()
    {
        
    }
    
    // add safety for finlastfo
    void OkStatusResponse::buildHeader(const std::string &fileName)
    {
        addHeaderField("Server", SERVER_NAME);
        addHeaderField("Range", "bytes");
        addHeaderField("Date", getTimeStamp());
        if (_bodyStream != nullptr && fileName.size() > 1)
        {
            getBodyStream()->seekg(0, std::ios_base::end);
		    addHeaderField("Content-Length", std::to_string(_bodyStream->tellg()));
            std::string fileExtensionWithoutDot = fileName.substr(fileName.find_last_of(".") + 1, fileName.size());
		    std::string mimeType = GlobalConfig::get().mimeTypes.getMIMEType(fileExtensionWithoutDot);
		    addHeaderField("Content-Type", mimeType);
        }
    }
}
