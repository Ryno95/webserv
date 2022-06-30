#include <criterion/criterion.h>
#include <iostream>
#include <Utility.hpp>

using namespace Webserver;

Test(prependRoot, rootWforwardSlash)
{
    std::string prepended(prependRoot("root/", "target/file.txt"));

    std::string expected("root/target/file.txt");

	cr_expect(prepended == expected);
}

Test(prependRoot, targetWforwardSlash)
{
    std::string prepended(prependRoot("root", "/target/file.txt"));
    
    std::string expected("root/target/file.txt");

	cr_expect(prepended == expected);
}

Test(prependRoot, targetAndRootWforwardSlash)
{
    std::string prepended(prependRoot("root/", "/target/file.txt"));
    
    std::string expected("root/target/file.txt");

	cr_expect(prepended == expected);
}

Test(prependRoot, NOforwardSlash)
{
    std::string prepended(prependRoot("root/", "/target/file.txt"));
    
    std::string expected("root/target/file.txt");

	cr_expect(prepended == expected);
}

Test(prependRoot, rootIncInRequest)
{
    std::string prepended(prependRoot("root/", "/root/target/file.txt"));
    
    std::string expected("root/target/file.txt");

	cr_expect(prepended == expected);
}
