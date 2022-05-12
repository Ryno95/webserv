#pragma once

#include <Response.hpp>

namespace Webserver
{
    class OKResponse: public Response
    {
        public:
            OKResponse();
            OKResponse(std::ifstream* stream);
            ~OKResponse();

        private:
           void buildHeader();
    };
}
