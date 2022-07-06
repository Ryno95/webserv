#include <criterion/criterion.h>
#include <Utility.hpp>

using namespace Webserver;

Test(Wildcard, Valid)
{
	cr_expect(wildcard("/root/cgi-bin/script.cgi", "*") == true);
	cr_expect(wildcard("/root/cgi-bin/script.cgi", "/root/cgi-bin/*.cgi") == true);
	cr_expect(wildcard("/root/cgi-bin/script.cgi", "*.cgi") == true);
	cr_expect(wildcard("/root/cgi-bin/script.cgi", "***") == true);
	cr_expect(wildcard("/root/cgi-bin/script.cgi", "*/roo*cgi-*/script*g*") == true);
}

Test(Wildcard, Invalid)
{
	cr_expect(wildcard("/root/cgi-bin/script.cgi", "*/oo*cgi-*/script*g*") == false);
	cr_expect(wildcard("/root/cgi-bin/script.cgi", "") == false);
}
