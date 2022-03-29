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

bool Client::recvRequest()
{
	int receivedBytes, prevSize;

	std::cout << "Reading on socket: " << _fd << std::endl;

	// Increase the buffer size: std::string::resize
	// Append to buffer with recv
	prevSize = _buffer.size();
	_buffer.resize(_buffer.size() + BUFFER_SIZE);
	receivedBytes = recv(_fd, &_buffer.front() + prevSize, BUFFER_SIZE, 0);
	std::cout << "RB: " << receivedBytes << std::endl;
	if (receivedBytes == 0) // This only works on Linux?
		return false;
	if (receivedBytes != SYSTEM_ERR)
	{
		// Check if the last portion of the request is received
		// The request seems to end on double CRLF: CR = Carriage Return (ascii 13) and LF = Line Feed (ascii 10)
		// So that means [13] [10] [13] [10] for the last 4 bytes in _buffer.
		if (_buffer.size() >= 4)
		{
			// This probably needs to be optimised
			// for example only check the last 4 bytes read instead of the whole buffer
			if (_buffer.find("\r\n\r\n") != std::string::npos)
			{
				// End of request received!
				std::cout << _buffer << std::endl;
			}
		}
	}
	else
		perror("Read error");
	return true;
}

bool Client::sendResponse()
{
	return true;
}
