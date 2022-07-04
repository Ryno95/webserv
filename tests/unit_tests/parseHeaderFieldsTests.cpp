#include <criterion/criterion.h>
#include <string>
#include <map>
#include <iostream>
#include <../includes/Exception.hpp>
#include <../includes/HeaderFieldParser.hpp>

using namespace Webserver;

Test(ParseHeaderTests, ValidHeader)
{
	const std::string input =	"User-Agent: libcurl/7.16.3\r\n"
								"Host: www.example.com\r\n"
								"Accept-Language: en, mi\r\n\r\n";

	HeaderFields fields;
	bool status = true;
	try 
	{
		fields = HeaderFieldParser().parse(input);
	}
	catch(const InvalidRequestException& e)
	{
		status = false;
	}

	cr_expect(fields._map["User-Agent"] == "libcurl/7.16.3");
	cr_expect(fields._map["Host"] == "www.example.com");
	cr_expect(fields._map["Accept-Language"] == "en, mi");
}

//  A server MUST reject any received request message that contains
//  whitespace between a header field-name and colon with a response code
// //  of 400 (Bad Request). A proxy MUST remove any such whitespace from a
// //  response message before forwarding the message downstream.
Test(ParseHeaderTests, WhiteSpaceBeforeColon)
{
	const std::string input = "User-Agent : libcurl/7.16.3\r\n\r\n";
	HeaderFields fields;
	bool status = true;

	try 
	{
		fields = HeaderFieldParser().parse(input);
	}
	catch(const InvalidRequestException& e)
	{
		status = false;
	}

	cr_expect(status == false);
}

Test(ParseHeaderTests, WhiteSpaceAfterColon)
{
	const std::string input = "User-Agent:       libcurl/7.16.3\r\n\r\n";

	HeaderFields fields;
	bool status = true;
	try 
	{
		fields = HeaderFieldParser().parse(input);
	}
	catch(const InvalidRequestException& e)
	{
		status = false;
	}

	cr_expect(fields._map["User-Agent"] == "libcurl/7.16.3");
	cr_expect(fields._map.size() == 1);
}

Test(ParseHeaderTests, WhiteSpaceBeforeKey)
{
	const std::string input = "       User-Agent: libcurl/7.16.3\r\n\r\n";

	HeaderFields fields;
	bool status = true;
	try 
	{
		fields = HeaderFieldParser().parse(input);
	}
	catch(const InvalidRequestException& e)
	{
		status = false;
	}

	cr_expect(fields._map["User-Agent"] == "libcurl/7.16.3");
}

Test(ParseHeaderTests, WhiteSpaceBeforeAndAfterValue)
{
	const std::string input = "User-Agent: libcurl/7.16.3    \r\n\r\n";

	HeaderFields fields;
	bool status = true;
	try 
	{
		fields = HeaderFieldParser().parse(input);
	}
	catch(const InvalidRequestException& e)
	{
		status = false;
	}

	cr_expect(fields._map["User-Agent"] == "libcurl/7.16.3");
	cr_expect(fields._map.size() == 1);
}

// // Might change as we continue
Test(ParseHeaderTests, EmptyHeaderString)
{
	const std::string input = "";

	HeaderFields fields;
	bool status = true;
	try 
	{
		fields = HeaderFieldParser().parse(input);
	}
	catch(const InvalidRequestException& e)
	{
		status = false;
	}

	cr_expect(status == true);
	cr_expect(fields._map.size() == 0);
}

Test(ParseHeaderTests, NoColon)
{
	const std::string input = "User-Agent  libcurl/7.16.3\r\n\r\n";

	HeaderFields fields;
	bool status = true;
	try 
	{
		fields = HeaderFieldParser().parse(input);
	}
	catch(const InvalidRequestException& e)
	{
		status = false;
	}

	cr_expect(status == false);
	cr_expect(fields._map.size() == 0);
}

Test(ParseHeaderTests, MultiLineNoColon)
{
	const std::string input = 	"User-Agent  libcurl/7.16.3\r\n"
								"Host: www.example.com\r\n\r\n";

	HeaderFields fields;
	bool status = true;
	try 
	{
		fields = HeaderFieldParser().parse(input);
	}
	catch(const InvalidRequestException& e)
	{
		status = false;
	}

	cr_expect(status == false);
	cr_expect(fields._map.size() == 0);
}

Test(ParseHeaderTests, ColonInHeader)
{
	const std::string input = 	"Host: localhost:8080\r\n\r\n";

	HeaderFields fields;
	bool status = true;
	try 
	{
		fields = HeaderFieldParser().parse(input);
	}
	catch(const InvalidRequestException& e)
	{
		status = false;
	}

	cr_expect(fields._map.size() == 1);
	cr_expect(fields._map["Host"] == "localhost:8080");

}

Test(HeaderFieldTests, CaseSensitivity)
{
	const std::string input =	"UsEr-AgEnT: libcurl/7.16.3\r\n"
								"HOST: www.example.com\r\n"
								"AccEpt-LangUagE: en, mi\r\n\r\n";

	HeaderFields fields;
	bool status = true;
	try 
	{
		fields = HeaderFieldParser().parse(input);
	}
	catch(const InvalidRequestException& e)
	{
		status = false;
	}

	cr_expect(fields._map["USER-AGENT"] == "libcurl/7.16.3");
	cr_expect(fields._map["host"] == "www.example.com");
	cr_expect(fields._map["accept-language"] == "en, mi");
}
