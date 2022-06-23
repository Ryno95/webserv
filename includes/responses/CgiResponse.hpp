#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <Cgi.hpp>
#include <Request.hpp>
#include <Host.hpp>
#include <responses/AResponse.hpp>
 
namespace Webserver
{
	class CgiResponse : public AResponse
	{
	private: 
		Cgi			_cgiRequest;
	
	public: 
	// Constructor 
		CgiResponse(const Request &request, const Host &host, const std::string& uri);
		~CgiResponse() {};

		std::istream *getBodyStream();

	};
}
