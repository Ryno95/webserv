#include <criterion/criterion.h>
#include <MimeTypes.hpp>

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

	bool boolean;
	try
	{
		mimes.getMIMEType("doesnt/exist");
		boolean = true;
	}
	catch(const std::exception& e)
	{
		boolean = false;
	}

	cr_expect(boolean == false);
}
