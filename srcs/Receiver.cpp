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

std::deque<Request> const& Receiver::getRequests()
{
	return _readyRequests;
}

void Receiver::receive()
{
	int receivedBytes, prevSize;

	prevSize = _buffer.size();
	_buffer.resize(_buffer.size() + BUFFER_SIZE);
	receivedBytes = recv(_fd, &_buffer.front() + prevSize, BUFFER_SIZE, 0);

	if (receivedBytes == 0)
		throw Client::DisconnectedException();
	if (receivedBytes == SYSTEM_ERR) // do we need to handle this?
		perror("Read error");
}


void Receiver::processHeaderRecv()
{
	if (_buffer.size() >= 4 && _buffer.find("\r\n\r\n") != std::string::npos)
		_state = CHECK_HEADER;
}

void Receiver::processBodyRecv()
{
	if (_bodyBytesReceived >= _bodySize)
	{
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
				break;

			case CHECK_HEADER:
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

				break;
		}

		if (prevState == _state)
			break;
	}
}
