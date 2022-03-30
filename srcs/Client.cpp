#include <Client.hpp>
#include <defines.hpp>

#include <unistd.h>
#include <sys/socket.h>

#include <iostream>

#define BUFFER_SIZE 9

Client::Client(int fd) : _fd(fd)
{
}

Client::~Client()
{
}

Client::state Client::recvRequest()
{
	int receivedBytes, prevSize;

	prevSize = _buffer.size();
	_buffer.resize(_buffer.size() + BUFFER_SIZE);
	receivedBytes = recv(_fd, &_buffer.front() + prevSize, BUFFER_SIZE, 0);

	if (receivedBytes == 0)
		return DISCONNECTED;
	if (receivedBytes != SYSTEM_ERR)
	{
			// This probably needs to be optimised
			// for example only check the last 4 bytes read instead of the whole buffer
		if (_buffer.size() >= 4 && _buffer.find("\r\n\r\n") != std::string::npos)
			return RECV_DONE;
	}
	else
		perror("Read error");
	return CONTINUE;
}

bool Client::handleRequest()
{
	state st = recvRequest();

	if (st == CONTINUE)
		return true;
	else if (st == DISCONNECTED)
		return false;
	Request request(_buffer);
	try
	{
		request.parse();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	return true;
}

bool Client::sendResponse()
{
	return true;
}
