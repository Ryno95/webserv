#pragma once

#include <string>
#include <vector>

class Client
{
public:
	Client(int fd);
	~Client();

	bool recvRequest();
	bool sendResponse();

private:
	int _fd;
	std::string _buffer;

};
