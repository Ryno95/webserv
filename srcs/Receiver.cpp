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
	Returns the currently prepared requests and removes those returned from the storage.
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

	_buffer = std::string();
	_buffer.resize(BUFFER_SIZE);
	receivedBytes = recv(_fd, &_buffer.front(), BUFFER_SIZE - 1, 0);

	if (receivedBytes == 0)
		throw Client::DisconnectedException();
	if (receivedBytes == SYSTEM_ERR) // do we need to handle this?
		perror("Read error");
}


void Receiver::processHeaderRecv()
{
	size_t pos = _buffer.find("\r\n\r\n");
	if (pos == std::string::npos)
		return;

	_received += _buffer.substr(0, pos - 1);
	pos += 3;
	_buffer = _buffer.substr(pos, strlen(_buffer.c_str()) - pos);
	_state = CHECK_HEADER;
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

	while (1)
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
				break;

			case CHECK_HEADER:
				checkHeader();
				break;
		}

		if (prevState == _state)
			break;
	}
}
