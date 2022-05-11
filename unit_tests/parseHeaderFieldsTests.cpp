#include <criterion/criterion.h>
#include <string>
#include <map>
#include <iostream>
#include <../includes/Request.hpp>

using namespace Webserver;

Test(ParseHeaderTests, ValidHeader)
{
	const std::string input =	"User-Agent: libcurl/7.16.3\r\n"
								"Host: www.example.com\r\n"
								"Accept-Language: en, mi\r\n\r\n";

	Request myRequest(input);
	try 
	{
		myRequest.parseHeaderFields(0);
	}
	catch(const std::exception& e) {}
	

	cr_expect(myRequest.getStatus() == HttpStatusCodes::OK);
	cr_expect(myRequest._headerFields["User-Agent"] == "libcurl/7.16.3");
	cr_expect(myRequest._headerFields["Host"] == "www.example.com");
	cr_expect(myRequest._headerFields["Accept-Language"] == "en, mi");
}

//  A server MUST reject any received request message that contains
//  whitespace between a header field-name and colon with a response code
// //  of 400 (Bad Request). A proxy MUST remove any such whitespace from a
// //  response message before forwarding the message downstream.
Test(ParseHeaderTests, WhiteSpaceBeforeColon)
{

	const std::string input = "User-Agent : libcurl/7.16.3\r\n\r\n";
	Request myRequest(input);
	try
	{
		myRequest.parseHeaderFields(0);
	}
	catch(const std::exception& e) {}
	cr_expect(myRequest.getStatus() == HttpStatusCodes::BAD_REQUEST);
}

Test(ParseHeaderTests, WhiteSpaceAfterColon)
{

	const std::string input = "User-Agent:       libcurl/7.16.3\r\n\r\n";
	Request myRequest(input);

	try
	{
		myRequest.parseHeaderFields(0);
	}
	catch(const std::exception& e) {}
	
	cr_expect(myRequest.getStatus() == HttpStatusCodes::OK);
	cr_expect(myRequest._headerFields["User-Agent"] == "libcurl/7.16.3");
	cr_expect(myRequest._headerFields.size() == 1);
}

Test(ParseHeaderTests, WhiteSpaceBeforeKey)
{

	const std::string input = "       User-Agent: libcurl/7.16.3\r\n\r\n";
	Request myRequest(input);

	try
	{
		myRequest.parseHeaderFields(0);
	}
	catch(const std::exception& e) {}
	cr_expect(myRequest.getStatus() == HttpStatusCodes::OK);
	cr_expect(myRequest._headerFields["User-Agent"] == "libcurl/7.16.3");
}

Test(ParseHeaderTests, WhiteSpaceBeforeAndAfterValue)
{

	const std::string input = "User-Agent: libcurl/7.16.3    \r\n\r\n";
	Request myRequest(input);

	try
	{
		myRequest.parseHeaderFields(0);
	}
	catch(const std::exception& e) {}
	
	cr_expect(myRequest.getStatus() == HttpStatusCodes::OK);
	cr_expect(myRequest._headerFields["User-Agent"] == "libcurl/7.16.3");
	cr_expect(myRequest._headerFields.size() == 1);
}

// // Might change as we continue
Test(ParseHeaderTests, EmptyHeaderString)
{

	const std::string input = "";
	Request myRequest(input);
	try
	{
		myRequest.parseHeaderFields(0);
	}
	catch(const std::exception& e) {}
	
	cr_expect(myRequest._headerFields.size() == 0);
	cr_expect(myRequest.getStatus() == HttpStatusCodes::OK);
}

Test(ParseHeaderTests, NoColon)
{

	const std::string input = "User-Agent  libcurl/7.16.3\r\n\r\n";
	Request myRequest(input);
	try
	{
		myRequest.parseHeaderFields(0);
	}
	catch(const std::exception& e) {}

	cr_expect(myRequest.getStatus() == HttpStatusCodes::BAD_REQUEST);
	cr_expect(myRequest._headerFields.size() == 0);
}

Test(ParseHeaderTests, MultiLineNoColon)
{

	const std::string input = 	"User-Agent  libcurl/7.16.3\r\n"
								"Host: www.example.com\r\n\r\n";
	Request myRequest(input);
	try
	{
		myRequest.parseHeaderFields(0);
	}
	catch(const std::exception& e) {}

	cr_expect(myRequest.getStatus() == HttpStatusCodes::BAD_REQUEST);
	cr_expect(myRequest._headerFields.size() == 0);
}

Test(ParseHeaderTests, ColonInHeader)
{

	const std::string input = 	"Host: localhost:8080\r\n\r\n";
	Request myRequest(input);
	try
	{
		myRequest.parseHeaderFields(0);
	}
	catch(const std::exception& e) {}

	cr_expect(myRequest.getStatus() == HttpStatusCodes::OK);
	cr_expect(myRequest._headerFields.size() == 1);
	cr_expect(myRequest._headerFields["Host"] == "localhost:8080");

}