#include <Client.hpp>
#include <defines.hpp>

#include <unistd.h>
#include <sys/socket.h>

#include <iostream>

#define BUFFER_SIZE 9

Client::Client(int fd) : _fd(fd), _requestReceiver(fd)
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
	_requestReceiver.handle();
}

#pragma endregion

#pragma region Response

// bool Client::sendResponse()
// {
// 	std::string buffer = HTTPVERSION;
// 	buffer += " ";
// 	buffer += std::to_string(_request.getStatus().first);
// 	buffer += " ";
// 	buffer += _request.getStatus().second;
// 	buffer += "\r\ncontent-length: 17\r\n\r\nSERVER GOES BRRRR";
// 	std::cout << "Send to " << _fd << ": " << std::endl << buffer << std::endl;
// 	send(_fd, buffer.c_str(), buffer.size(), 0);
// 	return true;
// }

/*
	Returns false when the client should be removed (disconnected)
*/
bool Client::handleResponse()
{
	return true;
}

#pragma endregion
