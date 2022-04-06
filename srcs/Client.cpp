#include <Client.hpp>
#include <defines.hpp>

#include <unistd.h>
#include <sys/socket.h>

#include <iostream>

Client::Client(int fd) : _fd(fd), _requestReceiver(fd), _responseSender(fd)
{
}

Client::~Client()
{
}

/*
	Returns false when the client should be removed (disconnected)
*/
bool Client::handleRequest()
{
	try
	{
		if (_requestReceiver.handle())
		{
			// add request to requests queue
			_responseSender.handle(_requestReceiver.getRequest()); // tmp function for program flow
		}
	}
	catch(const DisconnectedException& e)
	{
		std::cerr << e.what() << '\n';
		return false;
	}
	return true;
}

/*
	Returns false when the client should be removed (disconnected)
*/
bool Client::handleResponse()
{
	return true;
}
