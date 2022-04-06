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

#pragma region Request

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
		if (_buffer.size() >= 4 && _buffer.find("\r\n\r\n") != std::string::npos)
			return CHECK_HEADER;
	}
	else
		perror("Read error");
	return RECV_HEADER;
}

bool Client::isRecvState() const
{
	return _state == RECV_BODY || _state == RECV_HEADER;
}

/*
	Returns false when the client should be removed (disconnected)
*/
bool Client::handleRequest()
{
	_requestHandler.handle();
	while (true)
	{
		if (isRecvState())
		{
			_state = recvRequest();
			if (isRecvState())
				return true;
		}

		if (_state == CHECK_HEADER)
		{
			_request = Request(_buffer);

			try
			{
				_request.parse();
				_state = _request.checkForBody() ? RECV_BODY : RESPOND;
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
				_state = RESPOND;
			}
		}

		if (_state == DISCONNECTED)
			return false;
		else if (_state == RESPOND)
			return true;
	}
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
