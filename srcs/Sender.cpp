#include <Sender.hpp>
#include <defines.hpp>
#include <Logger.hpp>

#include <iostream>
#include <string>

#include <unistd.h>

#include <stack>

namespace Webserver
{
	Sender::Sender(int fd) : _fd(fd), _currentState(FINISHED), _response(nullptr), _buffer(new char[BUFFERSIZE])
	{
	}

	Sender::~Sender()
	{
		delete[] _buffer;
		deleteResponse();
	}

	void Sender::setStream()
	{
		switch (_currentState)
		{
			case SEND_HEADER:
				_sendStream = _response->getHeaderStream();
				break;

			case SEND_BODY:
				_sendStream = _response->getBodyStream();
				break;

			case FINISHED:
				_sendStream = nullptr;
				break;
		}
	}

	void Sender::handle()
	{
		uint	bufferBytesFilled = 0;
		uint	prevBytesFilled;

		while (bufferBytesFilled < BUFFERSIZE && _currentState != FINISHED)
		{
			prevBytesFilled = bufferBytesFilled;
			if (_sendStream == nullptr)
				setStream();

			if (_sendStream != nullptr)
			{
				bufferBytesFilled += _sendStream->read(_buffer + bufferBytesFilled, BUFFERSIZE - bufferBytesFilled);

				// If the stream is finished with filling (writing end) and the amount of bytes in the buffer is less than buffer size,
				// that means that the stream we are currently reading from is depleted.
				if (_sendStream->getIsFilled() && bufferBytesFilled < BUFFERSIZE)
				{
					_currentState++;
					_sendStream = nullptr;
				}
			}

			// (NON-BLOCKING) - No bytes have been added to the stream this iteration. the stream is not marked as finished, but is not ready to read on.
			if (bufferBytesFilled == prevBytesFilled)
				break;
		}

		if (bufferBytesFilled != 0)
		{
			ssize_t written;
			written = write(_fd, _buffer, bufferBytesFilled);
		}

		if (_currentState == FINISHED)
			deleteResponse();
	}

	void Sender::deleteResponse()
	{
		if (_response != nullptr)
		{
			delete _response;
			_response = nullptr;
		}
	}

	bool Sender::hasResponse() const
	{
		return _currentState != FINISHED;
	}

	void Sender::setResponse(Response* response)
	{
		_response = response;
		_currentState = SEND_HEADER;
		_sendStream = nullptr;
	}
}
