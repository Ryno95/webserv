#include <criterion/criterion.h>
#include <MimeTypes.hpp>

using namespace Webserver;

Test(Mimes, valid)
{
	MimeTypes mimes("config/test.mime");

	cr_expect(mimes.getMIMEType("html") == "text/html");
	cr_expect(mimes.getMIMEType("htm") == "text/html");
	cr_expect(mimes.getMIMEType("shtml") == "text/html");
	cr_expect(mimes.getMIMEType("mng") == "video/x-mng");
}

Test(Mimes, invalid)
{
	MimeTypes mimes("config/test.mime");

	cr_expect(mimes.getMIMEType("doenstExists") == "application/octet-stream");
	cr_expect(mimes.getMIMEType(".") == "application/octet-stream");
	cr_expect(mimes.getMIMEType("") == "application/octet-stream");
	cr_expect(mimes.getMIMEType("html.") == "application/octet-stream");
}
