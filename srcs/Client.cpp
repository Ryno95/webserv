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

#pragma region Request

/*
	Returns false when the client should be removed (disconnected)
*/
bool Client::handleRequest()
{
	try
	{
		if (_requestReceiver.handle())
		{
			_responseSender.handle(_requestReceiver.getRequest());
		}
	}
	catch(const DisconnectedException& e)
	{
		std::cerr << e.what() << '\n';
		return false;
	}
	return true;
}

#pragma endregion

#pragma region Response

/*
	Returns false when the client should be removed (disconnected)
*/
bool Client::handleResponse()
{
	return true;
}

#pragma endregion
