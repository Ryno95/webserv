#include <criterion/criterion.h>
#include <string>
#include <map>
#include <iostream>
#include <../includes/Request.hpp>
#include <../includes/Exception.hpp>
#include <../includes/RequestParser.hpp>

using namespace Webserver;

Test(ParseHeaderTests, ValidHeader)
{
	const std::string input =	"User-Agent: libcurl/7.16.3\r\n"
								"Host: www.example.com\r\n"
								"Accept-Language: en, mi\r\n\r\n";

	
	Request myRequest = RequestParser().parse(input);
	HttpStatusCode status;
	try 
	{
		myRequest.parseHeaderFields(0);
		status = HttpStatusCodes::OK;
	}
	catch(const InvalidRequestException& e)
	{
		status = e.getStatus();
	}

	cr_expect(status == HttpStatusCodes::OK);
	cr_expect(myRequest._map["User-Agent"] == "libcurl/7.16.3");
	cr_expect(myRequest._map["Host"] == "www.example.com");
	cr_expect(myRequest._map["Accept-Language"] == "en, mi");
}

//  A server MUST reject any received request message that contains
//  whitespace between a header field-name and colon with a response code
// //  of 400 (Bad Request). A proxy MUST remove any such whitespace from a
// //  response message before forwarding the message downstream.
Test(ParseHeaderTests, WhiteSpaceBeforeColon)
{

	const std::string input = "User-Agent : libcurl/7.16.3\r\n\r\n";
	Request myRequest = RequestParser().parse(input);
	HttpStatusCode status;
	try
	{
		myRequest.parseHeaderFields(0);
		status = HttpStatusCodes::OK;
	}
	catch(const InvalidRequestException& e)
	{
		status = e.getStatus();
	}
	cr_expect(status == HttpStatusCodes::BAD_REQUEST);
}

Test(ParseHeaderTests, WhiteSpaceAfterColon)
{

	const std::string input = "User-Agent:       libcurl/7.16.3\r\n\r\n";
	Request myRequest = RequestParser().parse(input);

	HttpStatusCode status;
	try
	{
		myRequest.parseHeaderFields(0);
		status = HttpStatusCodes::OK;
	}
	catch(const InvalidRequestException& e)
	{
		status = e.getStatus();
	}
	cr_expect(status == HttpStatusCodes::OK);
	cr_expect(myRequest._map["User-Agent"] == "libcurl/7.16.3");
	cr_expect(myRequest._map.size() == 1);
}

Test(ParseHeaderTests, WhiteSpaceBeforeKey)
{

	const std::string input = "       User-Agent: libcurl/7.16.3\r\n\r\n";
	Request myRequest = RequestParser().parse(input);
	HttpStatusCode status;

	try
	{
		myRequest.parseHeaderFields(0);
		status = HttpStatusCodes::OK;
	}
	catch(const InvalidRequestException& e)
	{
		status = e.getStatus();
	}
	cr_expect(status == HttpStatusCodes::OK);
	cr_expect(myRequest._map["User-Agent"] == "libcurl/7.16.3");
}

Test(ParseHeaderTests, WhiteSpaceBeforeAndAfterValue)
{

	const std::string input = "User-Agent: libcurl/7.16.3    \r\n\r\n";
	Request myRequest = RequestParser().parse(input);

	HttpStatusCode status;

	try
	{
		myRequest.parseHeaderFields(0);
		status = HttpStatusCodes::OK;
	}
	catch(const InvalidRequestException& e)
	{
		status = e.getStatus();
	}
	cr_expect(status == HttpStatusCodes::OK);
	cr_expect(myRequest._map["User-Agent"] == "libcurl/7.16.3");
	cr_expect(myRequest._map.size() == 1);
}

// // Might change as we continue
Test(ParseHeaderTests, EmptyHeaderString)
{

	const std::string input = "";
	Request myRequest = RequestParser().parse(input);
	HttpStatusCode status;

	try
	{
		myRequest.parseHeaderFields(0);
		status = HttpStatusCodes::OK;
	}
	catch(const InvalidRequestException& e)
	{
		status = e.getStatus();
	}
	cr_expect(myRequest._map.size() == 0);
	cr_expect(status == HttpStatusCodes::OK);
}

Test(ParseHeaderTests, NoColon)
{

	const std::string input = "User-Agent  libcurl/7.16.3\r\n\r\n";
	Request myRequest = RequestParser().parse(input);
	HttpStatusCode status;

	try
	{
		myRequest.parseHeaderFields(0);
		status = HttpStatusCodes::OK;
	}
	catch(const InvalidRequestException& e)
	{
		status = e.getStatus();
	}
	cr_expect(status == HttpStatusCodes::BAD_REQUEST);
	cr_expect(myRequest._map.size() == 0);
}

Test(ParseHeaderTests, MultiLineNoColon)
{

	const std::string input = 	"User-Agent  libcurl/7.16.3\r\n"
								"Host: www.example.com\r\n\r\n";
	Request myRequest = RequestParser().parse(input);
	HttpStatusCode status;

	try
	{
		myRequest.parseHeaderFields(0);
		status = HttpStatusCodes::OK;
	}
	catch(const InvalidRequestException& e)
	{
		status = e.getStatus();
	}

	cr_expect(status == HttpStatusCodes::BAD_REQUEST);
	cr_expect(myRequest._map.size() == 0);
}

Test(ParseHeaderTests, ColonInHeader)
{

	const std::string input = 	"Host: localhost:8080\r\n\r\n";
	Request myRequest = RequestParser().parse(input);
	HttpStatusCode status;

	try
	{
		myRequest.parseHeaderFields(0);
		status = HttpStatusCodes::OK;
	}
	catch(const InvalidRequestException& e)
	{
		status = e.getStatus();
	}

	cr_expect(status == HttpStatusCodes::OK);
	cr_expect(myRequest._map.size() == 1);
	cr_expect(myRequest._map["Host"] == "localhost:8080");

}

Test(HeaderFieldTests, CaseSensitivity)
{
	const std::string input =	"UsEr-AgEnT: libcurl/7.16.3\r\n"
								"HOST: www.example.com\r\n"
								"AccEpt-LangUagE: en, mi\r\n\r\n";

	Request myRequest = RequestParser().parse(input);
	HttpStatusCode status;
	myRequest.parseHeaderFields(0);

	cr_expect(myRequest._map["USER-AGENT"] == "libcurl/7.16.3");
	cr_expect(myRequest._map["host"] == "www.example.com");
	cr_expect(myRequest._map["accept-language"] == "en, mi");
}
