
#include <iostream>
#include <BadStatusResponse.hpp>
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
       _bodyStream = new std::ifstream(fileName);
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
        // addHeaderField("Content-Type: ", getTimeStamp()); How to dynamically get content type???
        addHeaderField("Server", SERVER_NAME);
        addHeaderField("Range", "bytes");
        addHeaderField("Date", getTimeStamp());
        // addHeaderField("content-type", "text/html, charset=utf-8");
        if (_bodyStream != nullptr)
        {
            getBodyStream()->seekg(0, std::ios_base::end);
		    addHeaderField("Content-Length", std::to_string(_bodyStream->tellg()));
        }
    }
}



// alt-svc: h3=":443"; ma=86400, h3-29=":443"; ma=86400
// cache-control: public, max-age=86400
// cf-cache-status: MISS
// cf-ray: 70a2fab19e324c7a-AMS
// content-encoding: gzip
// content-language: en
// content-type: text/html; charset=utf-8
// date: Thu, 12 May 2022 11:57:52 GMT
// expect-ct: max-age=604800, report-uri="https://report-uri.cloudflare.com/cdn-cgi/beacon/expect-ct"
// expires: Fri, 13 May 2022 11:57:52 GMT
// referrer-policy: no-referrer-when-downgrade
// server: cloudflare
// vary: Accept-Encoding
// vary: Accept-Language, Cookie
// x-backend: web-i-037c13073032ec260
// x-content-type-options: nosniff
// x-rtd-domain: docs.python-requests.org
// x-rtd-project: requests
// x-rtd-project-method: cname
// x-rtd-version: latest
// x-rtd-version-method: path
// x-served: Proxito-404
// x-xss-protection: 1; mode=block