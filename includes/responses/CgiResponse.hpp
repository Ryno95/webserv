#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <Cgi.hpp>
#include <Request.hpp>
#include <Host.hpp>
#include <methods/TargetInfo.hpp>
#include <responses/Response.hpp>
 
namespace Webserver
{
	class CgiResponse : public Response
	{
	private: 
		Cgi			_cgiRequest;
	
	public: 
		CgiResponse(const Request &request, const Host &host, const TargetInfo& uri);
		~CgiResponse();

	};
}
