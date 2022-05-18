#include <criterion/criterion.h>
#include <iostream>
#include "../includes/Uri.hpp"

using namespace Webserver;

Test(UriTests, AbsoluteValidTest1)
{
	Uri uri("http://google.com/homepage.html");

	cr_expect(uri.getResourcePath() == "/homepage.html");
	cr_expect(uri.getHost() == "google.com");
}

Test(UriTests, AbsoluteValidTest2)
{
	Uri uri("http://127.0.0.1/homepage.html");

	cr_expect(uri.getResourcePath() == "/homepage.html");
	cr_expect(uri.getHost() == "127.0.0.1");
}

Test(UriTests, AbsoluteValidTest3)
{
	Uri uri("http:///127.0.0.1/homepage.html");
	cr_expect(uri.getHost() == "");
	cr_expect(uri.getResourcePath() == "/127.0.0.1/homepage.html");
}

Test(UriTests, AbsoluteInvalidTest1)
{
	try
	{
		Uri uri(":http://127.0.0.1/homepage.html");
		cr_expect(false);
	}
	catch(const std::exception& e)
	{
		cr_expect(true);
	}
}

Test(UriTests, AbsoluteInvalidTest2)
{
	try
	{
		Uri uri("http://homepage.html");
		cr_expect(false);
	}
	catch(const std::exception& e)
	{
		cr_expect(true);
	}
}

Test(UriTests, RelativeValidTest1)
{
	Uri uri("/images/background.png");
	cr_expect(uri.isAbsolute() == false);
	cr_expect(uri.getResourcePath() == "/images/background.png");
}

Test(UriTests, RelativeValidTest2)
{
	Uri uri("/images//background.png");
	cr_expect(uri.isAbsolute() == false);
	cr_expect(uri.getResourcePath() == "/images/background.png");
}

Test(UriTests, RelativeValidTest3)
{
	Uri uri("/image");
	cr_expect(uri.isAbsolute() == false);
	cr_expect(uri.getResourcePath() == "/image");
}

Test(UriTests, RelativeValidTest4)
{
	Uri uri("/");
	cr_expect(uri.isAbsolute() == false);
	cr_expect(uri.getResourcePath() == "/");
}

Test(UriTests, InvalidTest1)
{
	Uri uri("/images///image.png");
	cr_expect(uri.isAbsolute() == false);
	cr_expect(uri.getResourcePath() == "/images/image.png");
}

Test(UriTests, InvalidTest2)
{
	try
	{
		Uri uri("images/image.png http://hello.html");
		cr_expect(false);
	}
	catch(const std::exception& e)
	{
		cr_expect(true);
	}
}

Test(UriTests, NormalizePath1)
{
	Uri uri("/../");
	std::cout << uri.getResourcePath() << std::endl;
 	cr_expect(uri.getResourcePath() == "/");
}

Test(UriTests, NormalizeInvalidPath)
{
	Uri uri("../");
	std::cout << uri.getResourcePath() << std::endl;
 	cr_expect(uri.getResourcePath() == "/");
}

Test(UriTests, NormalizeInvalidPathR)
{
	Uri uri("/root/uploads/../image.png");
	std::cout << uri.getResourcePath() << std::endl;
 	cr_expect(uri.getResourcePath() == "/root/image.png");
}

Test(UriTests, NormalizePath2)
{
	Uri uri("/../index.html");
	std::cout << uri.getResourcePath() << std::endl;
	cr_expect(uri.getResourcePath() == "/index.html");
}

Test(UriTests, NormalizePath3)
{
	Uri uri("/images/../images/image.png");
	std::cout << uri.getResourcePath() << std::endl;
	cr_expect(uri.getResourcePath() == "/images/image.png");
}

Test(UriTests, NormalizePath4)
{
	Uri uri("/images/../..///image.png");
	std::cout << uri.getResourcePath() << std::endl;
	cr_expect(uri.getResourcePath() == "/image.png");
}

Test(UriTests, NormalizePath5)
{
	Uri uri("/images/./.././images/pngs/../../images/pngs/image.png");
	std::cout << uri.getResourcePath() << std::endl;
	cr_expect(uri.getResourcePath() == "/images/pngs/image.png");
}

Test(UriTests, NormalizePath6)
{
	Uri uri("../..");
	std::cout << uri.getResourcePath() << std::endl;
	cr_expect(uri.getResourcePath() == "/");
}

Test(UriTests, NormalizePath7)
{
	Uri uri("./");
	std::cout << uri.getResourcePath() << std::endl;
	cr_expect(uri.getResourcePath() == "/");
}

Test(UriTests, NormalizePath8)
{
	Uri uri("/.");
	std::cout << uri.getResourcePath() << std::endl;
	cr_expect(uri.getResourcePath() == "/");
}

Test(UriTests, NormalizePath9)
{
	Uri uri("/....");
	std::cout << uri.getResourcePath() << std::endl;
	cr_expect(uri.getResourcePath() == "/....");
}

Test(UriTests, NormalizePath10)
{
	Uri uri("/..../...");
	std::cout << uri.getResourcePath() << std::endl;
	cr_expect(uri.getResourcePath() == "/..../...");
}
