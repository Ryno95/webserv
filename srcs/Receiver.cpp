#include <Receiver.hpp>
#include <defines.hpp>
#include <Client.hpp>
#include <Exception.hpp>

#include <unistd.h>
#include <sys/socket.h>

#include <stdexcept>
#include <iostream>

namespace Webserver
{
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

		_recvBuffer.resize(BUFFERSIZE);
		bytesRecv = recv(_fd, &_recvBuffer.front(), BUFFERSIZE, 0);
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
		while (_recvBuffer.size() > 0)
		{	
			_buffer += _recvBuffer.substr(0, 1);
			_recvBuffer = _recvBuffer.substr(1, _recvBuffer.size());
			if (_buffer.find("\r\n\r\n") != std::string::npos)
			{
				checkHeader();
				return ;
			}
		}
	}


	void Receiver::processBodyRecv()
	{
		_bodyBytesReceived += _recvBuffer.length();
		_newRequest.appendBody(_recvBuffer);
		_recvBuffer.resize(0);
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
				_bodySize = _newRequest.getBodySize(); // body size < max body size?
				return ;
			}
		}
		catch(const InvalidRequestException& e)
		{
			_newRequest.setStatus(e.getStatus());
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
					break;
			}
			if ((_state == RECV_HEADER || _state == RECV_BODY) && _recvBuffer.size() == 0)
				break ;
		}
	}
}
