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

				// The stream is completely filled, but we have not read all bytes from it.
				// That means we have read every byte in the buffer, so we can continue to our next task.
				if (_sendStream->getIsFilled() && bufferBytesFilled != BUFFERSIZE)
				{
					_currentState++;
					_sendStream = nullptr;
				}
			}

			// (NON-BLOCKING) - No bytes have been added to the stream this iteration. the stream is not marked as finished, but was not ready to read on.
			if (bufferBytesFilled == prevBytesFilled)
				break;
		}

		// remove this, it's a possible leak because it can prevent deleteResponse() from being called.
		// see end of this function!
		if (bufferBytesFilled != 0)
		{
			ssize_t written;
			written = write(_fd, _buffer, bufferBytesFilled);

			DEBUG("Sent " << written << " bytes to " << _fd);
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
