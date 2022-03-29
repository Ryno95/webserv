#pragma once

#include <string>

class Request
{
public:
	Request(std::string uri);
	~Request();

private:
	void parseURI();

	std::string _uri;

};
