#pragma once
#include <iostream>
#include <string>
#include <Cgi.hpp>
#include <Request.hpp>
#include <Host.hpp>
#include <responses/Response.hpp>
 
namespace Webserver
{
	class CgiResponse: public Response
	{
		private: 
			Cgi			_cgiRequest;
		
		public: 
		// Constructor 
			CgiResponse(const Request &request, const Host &host);
			~CgiResponse() {};
	};
	 
}
