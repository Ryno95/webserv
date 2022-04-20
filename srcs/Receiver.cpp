#include <Receiver.hpp>
#include <defines.hpp>
#include <Client.hpp>

#include <unistd.h>
#include <sys/socket.h>

#include <stdexcept>
#include <iostream>

Receiver::Receiver(int fd)
	: _fd(fd), _state(RECV_HEADER), _bodyBytesReceived(0), _bodySize(0)
{
}

Receiver::~Receiver()
{
}

/*
	Returns the currently prepared requests and removes those from the object's storage.
*/
std::deque<Request> const Receiver::collectRequests()
{
	std::deque<Request> current = _readyRequests;
	_readyRequests.clear();
	return current;
}

void Receiver::receive()
{
	int bytesRecv;

	_recvBuffer.resize(BUFFER_SIZE);
	bytesRecv = recv(_fd, &_recvBuffer.front(), BUFFER_SIZE, 0);

	if (bytesRecv == 0)
		throw Client::DisconnectedException();
	if (bytesRecv == SYSTEM_ERR) // do we need to handle this?
	{
		perror("Read error");
		bytesRecv = 0;
	}
	_recvBuffer.resize(bytesRecv);
}

void Receiver::processHeaderRecv()
{
	size_t pos = _recvBuffer.find("\r\n\r\n");
	if (pos == std::string::npos)
	{
		_buffer += _recvBuffer;
		_recvBuffer.resize(0);
	}
	else
	{
		pos += 4;
		_buffer += _recvBuffer.substr(0, pos);
		_recvBuffer = _recvBuffer.substr(pos, _recvBuffer.size() - pos);
		checkHeader();
	}
}



void Receiver::processBodyRecv()
{
	// std::cout << "Body size: " << _bodySize << std::endl;

	// std::cout << "ENTERING PROCESS BODY: " << _recvBuffer << std::endl;
	_bodyBytesReceived += _recvBuffer.length();
	_newRequest.appendBody(_recvBuffer);
	// std::cout << "ENTERING PROCESS BODY: " << _bodyBytesReceived << std::endl;
	if (_bodyBytesReceived >= _bodySize)
		_state = ADD_REQUEST;
}

void Receiver::checkHeader()
{
	_newRequest = Request(_buffer);
	_buffer.clear();

	try
	{
		_newRequest.parse();
		if (_newRequest.hasBodyField())
		{
			_state = RECV_BODY;
			_bodyBytesReceived = 0;
			_bodySize = _newRequest.getBodySize();
			// std::cout << "BODYSIZE: " << _bodySize << std::endl;
			// std::cout << "BODYContent: " << _recvBuffer << std::endl;
			return ;
		}
	}
	catch(const std::exception& e) // only catch parse exceptions?
	{
		std::cerr << e.what() << '\n';
	}
	_state = ADD_REQUEST;
}

/*
	Returns whether or not the handling is finished and ready to be collected.
*/
void Receiver::handle()
{
	receive();

	while (1)
	{
		std::cout << "INFINITY!\n";
		std::cout << "State: " << _state <<std::endl;
		std::cout << "Size : " << _recvBuffer.size() <<std::endl;
		switch (_state)
		{
			case RECV_HEADER:
				processHeaderRecv();
				break;

			case RECV_BODY:
				processBodyRecv();
				break;

			case ADD_REQUEST:
				_readyRequests.push_back(_newRequest);
				_state = RECV_HEADER;
				_recvBuffer.resize(0);
				std::cout << "Added request to queue!" << std::endl;
				break;
		}
		if ((_state == RECV_HEADER || _state == RECV_BODY) && _recvBuffer.size() == 0)
		{
			std::cout << "Breaking outta this place!\n";
			break ;
		}
	}
}
