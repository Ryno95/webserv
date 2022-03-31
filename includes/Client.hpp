#pragma once

#include <string>
#include <vector>

#include <Request.hpp>
#include <Response.hpp>

class Client
{

typedef enum state
{
	RECV_HEADER,
	RECV_BODY,
	DISCONNECTED,
	PROCESS,
} state;

public:
	Client(int fd);
	~Client();

	bool handleRequest();
	bool sendResponse();
	state recvRequest();

private:
	int			_fd;
	std::string	_buffer;

};
