#include <criterion/criterion.h>
#include <iostream>
#include <FileInfo.hpp>

using namespace Webserver;

Test(FileInfoTests, rootWforwardSlash)
{
	FileInfo fileInfo("target/file.txt", "root/");

	std::string expected("root/target/file.txt");
	std::cerr << "Path: " << fileInfo.getFullPath() << std::endl;

	cr_expect(fileInfo.getFullPath() == expected);
}

Test(FileInfoTests, targetWforwardSlash)
{
	FileInfo fileInfo("/target/file.txt", "root");
	
	std::string expected("root/target/file.txt");
	std::cerr << "Path: " << fileInfo.getFullPath() << std::endl;

	cr_expect(fileInfo.getFullPath() == expected);
}

Test(FileInfoTests, targetAndRootWforwardSlash)
{
	FileInfo fileInfo("/target/file.txt", "root/");
	
	std::string expected("root/target/file.txt");
	std::cerr << "Path: " << fileInfo.getFullPath() << std::endl;

	cr_expect(fileInfo.getFullPath() == expected);
}

Test(FileInfoTests, NOforwardSlash)
{
	FileInfo fileInfo("/target/file.txt", "root/");
	
	std::string expected("root/target/file.txt");
	std::cerr << "Path: " << fileInfo.getFullPath() << std::endl;

	cr_expect(fileInfo.getFullPath() == expected);
}

Test(FileInfoTests, rootIncInRequest)
{
	FileInfo fileInfo("/root/target/file.txt", "root/");
	
	std::string expected("root/target/file.txt");
	std::cerr << "Path: " << fileInfo.getFullPath() << std::endl;

	cr_expect(fileInfo.getFullPath() == expected);
}
