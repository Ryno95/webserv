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

	void Sender::setDataStream()
	{
		switch (_currentState)
		{
			case SEND_HEADER:
				_dataStream = _response->getHeaderStream();
				break;

			case SEND_BODY:
				_dataStream = _response->getBodyStream();
				if(_dataStream == nullptr)
					_dataStream = _response->_cgiStream;
				break;

			case FINISHED:
				std::cerr << "FINSIHED" << std::endl;
				_dataStream = nullptr;
				break;
		}
	}

	long Sender::fillBuffer(long bufferSize)
	{
		if (_dataStream == nullptr)
		{
			if (_currentState != SEND_BODY)
			{
				WARN("THIS SHOULD NOT OCCUR!");
			}
			return 0;
		}

		_dataStream->read(_buffer + bufferSize, BUFFERSIZE - bufferSize);
		return _dataStream->gcount();
	}

	void Sender::handle()
	{
		long	bufferSize = 0;
		int		bytes = 0;

		while (bufferSize < BUFFERSIZE && _currentState != FINISHED)
		{
			if (_dataStream == nullptr)
				setDataStream();
			bytes = fillBuffer(bufferSize);
			bufferSize += bytes;
			if (bufferSize < BUFFERSIZE)
			{
				_currentState++;
				_dataStream = nullptr;
				break ;
			}
		}


		if (bufferSize == 0 )
		{
			DEBUG("No bytes to send!");
			return;
		}

		if (bufferSize > BUFFERSIZE)
		{
			WARN("UNEXPECTED AMOUNT OF BYTES STORED IN THE BUFFER! (Sender.cpp)"); // DEBUG LINE
		}

		ssize_t written;
		written = write(_fd, _buffer, bufferSize);

		DEBUG("Sent " << written << " bytes to " << _fd);

		if (written != bufferSize || written == 0)
		{
			WARN("Actual bytes written is not equal to the amount requested to send." << std::endl <<
					"There is no implementation to catch this issue yet." << std::endl <<
					"Requested: " << bufferSize << " written: " << written << std::endl);
		}

		if (_currentState == FINISHED)
		{
			deleteResponse();
		}
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
		_dataStream = nullptr;
	}
}
