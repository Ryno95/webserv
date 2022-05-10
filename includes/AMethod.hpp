#pragma once

#include <Request.hpp>
#include <Response.hpp>

#include <iostream>
#include <string>

/*
Methods:

	GET:
		- optional body, mostly not
		- returns file at target URI
		
		1. Open file
		2. Read file
		3. Add data to response body
		4. Set response status

	POST:
		- with body
		- execute file at target URI
		- returns result

		1. Open file or create file
		2. Append to file
		3. 

		// Parsing a POST request
		If you are sending data with your POST request, you must provide the Content-Type and Content-Length HTTP headers that indicate the type and size of the data in your POST message.
		These HTTP headers will help the server interpret and process the sent data correctly.
		Alternatively, you can omit the Content-Length header for your POST request and use the Transfer-Encoding header instead.
		If the Content-Length and Transfer-Encoding headers are missing, the connection MUST be closed at the end of the response.
	
	DELETE:
		- optional body, mostly not
		- deletes file/entry:
			file if URI is target
			entry if URI is CGI
*/
namespace Webserver
{
	class AMethod
	{
	public:
		AMethod(Request const& request);
		virtual Response* process() = 0;

		void writeFile();

	protected:
		Request const& _request;
		Response* _response;
	};
}
