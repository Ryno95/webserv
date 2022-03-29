#include <criterion/criterion.h>
#include <string>
#include <map>
#include <iostream>
#include <../includes/Request.hpp>

const std::string input = 	"User-Agent: libcurl/7.16.3\n"
							"Host: www.example.com\n"
							"Accept-Language: en, mi\n";

Test(ParseHeaderTests, ValidHeader)
{
	Request myRequest(input);

	cr_expect(myRequest._headerFields["User-Agent"] == "libcurl/7.16.3");
	cr_expect(myRequest._headerFields["Host"] == "www.example.com");
	cr_expect(myRequest._headerFields["Accept-Language"] == "en, mi");
}

//  A server MUST reject any received request message that contains
//  whitespace between a header field-name and colon with a response code
//  of 400 (Bad Request). A proxy MUST remove any such whitespace from a
//  response message before forwarding the message downstream.
Test(ParseHeaderTests, WhiteSpaceBeforeColon)
{

	const std::string localInput = "User-Agent : libcurl/7.16.3\n";
	Request myRequest(localInput);

	cr_expect(myRequest.parseHeaderFields() == BAD_REQUEST);
}

// Might change as we continue
Test(ParseHeaderTests, EmptyHeaderString)
{

	const std::string localInput = "";
	Request myRequest(localInput);

	cr_expect(myRequest.parseHeaderFields() == OK);
}