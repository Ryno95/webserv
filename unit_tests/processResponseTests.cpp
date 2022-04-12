#include <criterion/criterion.h>
#include <string>
#include <map>
#include <iostream>
#include <Request.hpp>
#include <Response.hpp>

Test(ResponseTests, BasicGETRequest)
{
    const std::string seed =    "GET / HTTP/1.1\r\n"
                                "Host: localhost:8080\r\n"
                                "User-Agent: curl/7.54.0\r\n"
                                "Accept: */*\r\n\r\n";
    Request request(seed);

	request.parse();
    Response response(request._status);
	response.addHeaderFields();
	response.setBody("Hello Mr.Client");
	
	cr_expect(response._statusCode == HttpStatusCodes::OK);
	cr_expect(response._body == "Hello Mr.Client");
	cr_expect(response._headerFields["Server"] == "Simply the best");
	cr_expect(response._headerFields["Content-Length"] == "15");
}

