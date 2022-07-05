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

	void Receiver::processBodyContentLength()
	{
		_bodyBytesReceived += _recvBuffer.length();
		_newRequest.appendBody(_recvBuffer);
		_recvBuffer.resize(0);
		if (_bodyBytesReceived >= _bodySize)
			_state = ADD_REQUEST;
	}

	void Receiver::processBodyChunked()
	{
		// _bodyBytesReceived	= amount of bytes received in this chunk
		// _bodySize			= amount to receive in this chunk

		_buffer += _recvBuffer;
		_recvBuffer.clear();

		size_t pos = 0;

		while (true)
		{
			if (_bodySize == 0) // get next chunk size
			{
				if ((pos = _buffer.find("\r\n")) == std::string::npos)
					break; // First need to receive more bytes from the client


				std::stringstream stream;
				stream << std::hex << _buffer.substr(0, pos);
				stream >> _bodySize;

				_buffer.erase(0, pos + 2);

				if (_bodySize == 0)
				{
					_state = ADD_REQUEST;
					break;
				}
			}
			else if (_buffer.size() >= _bodySize + 2)
			{
				if (_buffer.substr(_bodySize, 2) != "\r\n")
					throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);

				_newRequest.appendBody(_buffer.substr(0, _bodySize));
				_buffer.erase(0, _bodySize + 2);

				_bodySize = 0;
			}
			else
				break; // First need to receive more bytes from the client
		}
	}

	bool Receiver::requestHasContentLength()
	{
		_bodySize = _newRequest.getBodySize();
		if (_bodySize > Webserv::config().getMaxRequestBodySize())
			throw InvalidRequestException(HttpStatusCodes::PAYLOAD_TOO_LARGE);
		else if (_bodySize == 0)
			return false;

		_processBodyFunction = &Receiver::processBodyContentLength;
		return true;
	}

	bool Receiver::requestHasChunkedEncoding()
	{
		std::string encoding;
		if (_newRequest.tryGetHeader(Header::TransferEncoding, encoding) && stringToLower(encoding) == "chunked")
		{
			_processBodyFunction = &Receiver::processBodyChunked;
			return true;
		}
		return false;
	}

	void Receiver::checkHeader()
	{
		try
		{
			_newRequest = RequestParser().parse(_buffer);
			_buffer.clear();

			_bodyBytesReceived = 0;
			_bodySize = 0;
			if (requestHasChunkedEncoding() || requestHasContentLength())
			{
				_state = RECV_BODY;
				return;
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
		Reads on it's configured fd for [buffer_size] bytes and then processes the bytes it has read.
		Fully received requests are stored in this object and can be retrieved using Receiver::collectRequests().
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
					(this->*_processBodyFunction)();
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
