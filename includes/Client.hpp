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
	CHECK_HEADER,
	DISCONNECTED,
	RESPOND
} state;

public:
	Client(int fd);
	~Client();

	bool	handleRequest();
	bool	handleResponse();

private:
	int			_fd;
	state		_state;
	Request		_request;
	Response	_response;
	std::string	_buffer;
	std::string	_sendBuffer;

	bool	isRecvState() const;
	state	recvRequest();
	bool	sendResponse();
};
