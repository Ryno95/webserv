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
	int receivedBytes;

	_recvBuffer.assign(BUFFER_SIZE, 0);
	// _recvBuffer.clear();
	// _recvBuffer.resize(BUFFER_SIZE);
	receivedBytes = recv(_fd, &_recvBuffer.front(), BUFFER_SIZE - 1, 0);

	if (receivedBytes == 0)
		throw Client::DisconnectedException();
	if (receivedBytes == SYSTEM_ERR) // do we need to handle this?
		perror("Read error");
}

void Receiver::processHeaderRecv()
{
	size_t pos = _recvBuffer.find("\r\n\r\n");
	if (pos == std::string::npos)
		_buffer += _recvBuffer;
	else
	{
		_buffer += _recvBuffer.substr(0, pos);
		pos += 4;
		_recvBuffer = _recvBuffer.substr(pos, _recvBuffer.size() - pos);
		_state = CHECK_HEADER;
	}
}

void Receiver::processBodyRecv()
{
	if (_bodyBytesReceived >= _bodySize)
	{
		_state = ADD_REQUEST;
	}
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
			// _bodySize = get from header field!
			throw std::runtime_error("NOT IMPLEMENTED YET!");
		}
		else
			_state = ADD_REQUEST;
	}
	catch(const std::exception& e) // only catch parse exceptions?
	{
		std::cerr << e.what() << '\n';
		_state = ADD_REQUEST;
	}
}

/*
	Returns whether or not the handling is finished and ready to be collected.
*/
void Receiver::handle()
{
	state prevState;

	receive();

	if (_state != RECV_HEADER && _state != RECV_BODY) // reset state
	{
		_state = RECV_HEADER;
		std::cout << "Resetting state" << std::endl;
	}

	while (_recvBuffer.size() > 0)
	{
		prevState = _state;

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
				std::cout << "Added request to queue!" << std::endl;
				_state = RECV_HEADER;
				break;

			case CHECK_HEADER:
				checkHeader();
				break;
		}

		if (prevState == _state)
			break;
	}
}
