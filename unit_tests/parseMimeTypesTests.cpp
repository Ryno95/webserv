#include <criterion/criterion.h>
#include <MimeTypes.hpp>
#include <Webserv.hpp>
#include <config/AppConfigParser.hpp>
#include <config/Parser.hpp>

using namespace Webserver;

AppConfig* parseConfig(const std::string& path)
{
	try
	{
		std::ifstream fstream(path);
		if (fstream.fail())
			throw FileNotFoundException(path);
		return Parser<AppConfigParser>(fstream).parse();
	}
	catch(const std::exception& e)
	{
		throw std::runtime_error(std::string("In config file: ") + e.what());
	}
}

Test(Mimes, valid)
{
	AppConfig* config = parseConfig("unit_tests/default.config");
	Webserv::config(config);
	MimeTypes mimes("unit_tests/test.mime");

	cr_expect(mimes.getMIMEType("html") == "text/html");
	cr_expect(mimes.getMIMEType("htm") == "text/html");
	cr_expect(mimes.getMIMEType("shtml") == "text/html");
	cr_expect(mimes.getMIMEType("mng") == "video/x-mng");
}

Test(Mimes, invalid)
{
	AppConfig* config = parseConfig("unit_tests/default.config");
	Webserv::config(config);
	MimeTypes mimes("unit_tests/test.mime");

	cr_expect(mimes.getMIMEType("doenstExists") == "application/octet-stream");
	cr_expect(mimes.getMIMEType(".") == "application/octet-stream");
	cr_expect(mimes.getMIMEType("") == "application/octet-stream");
	cr_expect(mimes.getMIMEType("html.") == "application/octet-stream");
}
