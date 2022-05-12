
#include <iostream>
#include <responses/BadStatusResponse.hpp>
#include <Utility.hpp>
#include <Logger.hpp>
#include <config/GlobalConfig.hpp>

namespace Webserver
{
    BadStatusResponse::BadStatusResponse() : Response()
    {
        buildHeader();
    }

    BadStatusResponse::BadStatusResponse(HttpStatusCode code) : Response(code)
    {
       std::string fileName("root/error.html");
       _bodyStream = new std::ifstream(NotFoundErrorPage);
       if (!_bodyStream->is_open())
            throw std::runtime_error("Errorfile doens't exist");
       std::string mimeType = GlobalConfig::get().mimeTypes.getMIMEType(fileName.substr(fileName.find_last_of(".") + 1, fileName.size()));
       addHeaderField("Content-Type", mimeType);
       buildHeader();
    }
    
    BadStatusResponse::~BadStatusResponse()
    {
    }
    
    void BadStatusResponse::buildHeader()
    {
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
