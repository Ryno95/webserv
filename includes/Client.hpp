#pragma once

#include <string>
#include <vector>

#include <Request.hpp>
#include <Response.hpp>

class Client
{

typedef enum state
{
	CONTINUE,
	DISCONNECTED,
	RECV_DONE
} state;

public:
	Client(int fd);
	~Client();

	bool handleRequest(); // Ryno wilt geen boolean
	bool sendResponse();
	state recvRequest();

private:
	int			_fd;
	std::string	_buffer;

};
