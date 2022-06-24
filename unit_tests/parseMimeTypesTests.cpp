#include <criterion/criterion.h>
#include <MimeTypes.hpp>
#include <Webserv.hpp>
#include <config/AppConfigParser.hpp>
#include <config/Parser.hpp>

using namespace Webserver;

// Test(Mimes, valid)
// {
// 	AppConfig* config = Parser<AppConfigParser>(new std::fstream("default.config")).parse();
// 	Webserv::config(config);
// 	MimeTypes mimes("unit_tests/test.mime");

// 	cr_expect(mimes.getMIMEType("html") == "text/html");
// 	cr_expect(mimes.getMIMEType("htm") == "text/html");
// 	cr_expect(mimes.getMIMEType("shtml") == "text/html");
// 	cr_expect(mimes.getMIMEType("mng") == "video/x-mng");
// }

// Test(Mimes, invalid)
// {
// 	AppConfig* config = Parser<AppConfigParser>(new std::fstream("default.config")).parse();
// 	Webserv::config(config);
// 	MimeTypes mimes("unit_tests/test.mime");

// 	cr_expect(mimes.getMIMEType("doenstExists") == "application/octet-stream");
// 	cr_expect(mimes.getMIMEType(".") == "application/octet-stream");
// 	cr_expect(mimes.getMIMEType("") == "application/octet-stream");
// 	cr_expect(mimes.getMIMEType("html.") == "application/octet-stream");
// }
