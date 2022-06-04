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
				usleep(2000);
				_dataStream = _response->getBodyStream();
				if(_dataStream == nullptr)
				{
					DEBUG("DATA STREAM SET TO CGI_STREAM");
					_dataStream = _response->_cgiStream;
					DEBUG("FULL CGI DATA: " << _dataStream->rdbuf());
					if (_dataStream != nullptr)
						IS_FINISHED = false;
					else
					{
						ERROR("No CGI datastream set!");
					}
				}
				break;

			case FINISHED:
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
			DEBUG("BYTESREAD: " << bytes);
			DEBUG("BUFFERSIZE: " << bufferSize);
			if (bufferSize < BUFFERSIZE)
			{
				ERROR("Incrementing state from " << _currentState << " to: " << _currentState + 1);
				_currentState++;
				_dataStream = nullptr;
			}
			// if (bytes == 0)
			// 	break ;
		}

		// this doesnt make sense, as soon as the buffer 
		if (_currentState == FINISHED)
		{
			deleteResponse();
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
		DEBUG("WRITING: " << _buffer);
		written = write(_fd, _buffer, bufferSize);

		DEBUG("Sent " << written << " bytes to " << _fd);

		if (written != bufferSize)
		{
			WARN("Actual bytes written is not equal to the amount requested to send." << std::endl <<
					"There is no implementation to catch this issue yet." << std::endl <<
					"Requested: " << bufferSize << " written: " << written << std::endl);
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
	bool Sender::IS_FINISHED = false;
}
