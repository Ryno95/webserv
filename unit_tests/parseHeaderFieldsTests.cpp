#include <criterion/criterion.h>
#include <string>
#include <map>
#include <iostream>
#include "../includes/Request.hpp"

const std::string input = 	"User-Agent: curl/7.16.3 libcurl/7.16.3\n"
							"Host: www.example.com\n"
							"Accept-Language: en, mi\n";

std::map<std::string, std::string> results;

void initResultMap()
{
	results.insert(std::pair<std::string, std::string>("User-Agent", "curl/7.16.3 libcurl/7.16.3");
}

Test(ParseTests, resultsCheck)
{
	cr_expect(results["User-Agent"] == "libcurl/7.16.3");
}