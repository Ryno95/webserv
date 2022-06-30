#include <Receiver.hpp>
#include <RequestParser.hpp>
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
		if (bytesRecv == SYSTEM_ERR)
			bytesRecv = 0;
		_recvBuffer.resize(bytesRecv);
	}

	void Receiver::processHeaderRecv()
	{
		size_t pos;

		_buffer += _recvBuffer;
		_recvBuffer.clear();
		pos = _buffer.find("\r\n\r\n");
		if (pos != std::string::npos)
		{
			_recvBuffer = _buffer.substr(pos + 4);
			_buffer = _buffer.erase(pos + 4);
			checkHeader();
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

	bool Receiver::requestHasBodyField(const Request& req)
	{
		_bodySize = req.getBodySize();
		if (_bodySize > Webserv::config().getMaxRequestBodySize())
			throw InvalidRequestException(HttpStatusCodes::PAYLOAD_TOO_LARGE);
		else if (_bodySize == 0)
			return false;
		return true;
	}

	void Receiver::checkHeader()
	{
		try
		{
			_newRequest = RequestParser().parse(_buffer);
			_buffer.clear();
			if (requestHasBodyField(_newRequest))
			{
				_state = RECV_BODY;
				_bodyBytesReceived = 0;
				return ;
			}
		}
		catch(const InvalidRequestException& e)
		{
			_newRequest.setStatus(e.getStatus());
			_buffer.clear();
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
