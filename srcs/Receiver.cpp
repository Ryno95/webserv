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
	// _recvBuffer.reserve(BUFFER_SIZE);
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
	// std::cout << "Recv: " << bytesRecv << std::endl;
	// std::cout << _recvBuffer << std::endl;
}

void Receiver::processHeaderRecv()
{
	size_t pos = _recvBuffer.find("\r\n\r\n");
	if (pos == std::string::npos)
	{
		std::cout << "NO end of header found." << std::endl;
		_buffer += _recvBuffer;
	}
	else
	{
		std::cout << "YES end of header found." << std::endl;
		_buffer += _recvBuffer.substr(0, pos);
		pos += 4;
		_recvBuffer = _recvBuffer.substr(pos, _recvBuffer.size() - pos);
		_state = CHECK_HEADER;
	}
	std::cout << "buffer: " << _buffer << std::endl;
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
	std::cout << "Check header" << std::endl;
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
			throw std::runtime_error("NOT IMPLEMENTED YET!"); // tmp throw till implemented
			return;
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
	state prevState;

	receive();

	if (_state != RECV_HEADER && _state != RECV_BODY)
		throw std::runtime_error("STATE SHOULD NOT OCCUR!");

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
				_state = RECV_HEADER;
				std::cout << "Added request to queue!" << std::endl;
				if (_recvBuffer.size() == 0)
					return;
				break;

			case CHECK_HEADER:
				checkHeader();
				break;
		}

		if (prevState == _state)
			break;
	}
}
