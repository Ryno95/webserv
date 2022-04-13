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
	
	DELETE:
		- optional body, mostly not
		- deletes file/entry:
			file if URI is target
			entry if URI is CGI
*/
class AMethod
{
public:
	AMethod(Request const& request);
	virtual Response process() = 0;

	void readFile();
	void writeFile();

private:
	Request const& _request;
	Response _response;
};
