#include <Sender.hpp>
#include <defines.hpp>
#include <Logger.hpp>

#include <iostream>
#include <string>

#include <unistd.h>

#include <stack>

Sender::Sender(int fd) : _fd(fd), _currentState(FINISHED), _response(nullptr), _buffer(new char[BUFFERSIZE])
{
}

Sender::Sender(Sender const& rhs) : _currentState(FINISHED), _response(nullptr), _buffer(new char[BUFFERSIZE])
{
	*this = rhs;
}

Sender::~Sender()
{
	delete[] _buffer;
	deleteResponse();
}

Sender& Sender::operator=(Sender const& rhs)
{
	_fd = rhs._fd;
	return *this;
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
			WARN("THIS SHOULD NOT OCCUR!");
		return 0;
	}

	_dataStream->read(_buffer + bufferSize, BUFFERSIZE - bufferSize);
	DEBUG("Read: " << _dataStream->gcount());
	return _dataStream->gcount();
}

void Sender::handle()
{
	long bufferSize = 0;

	while (bufferSize < BUFFERSIZE && _currentState != FINISHED)
	{
		if (_dataStream == nullptr)
			setDataStream();
		bufferSize += fillBuffer(bufferSize);
		if (bufferSize < BUFFERSIZE)
		{
			_currentState++;
			_dataStream = nullptr;
		}
	}

	if (_currentState == FINISHED)
		deleteResponse();

	if (bufferSize == 0)
	{
		DEBUG("No bytes to send!");
		return;
	}

	if (bufferSize > BUFFERSIZE)
		throw std::runtime_error("UNEXPECTED AMOUNT OF BYTES STORED IN THE BUFFER! (Sender.cpp)"); // DEBUG LINE

	ssize_t written;
	written = write(_fd, _buffer, bufferSize);

	DEBUG("Sent " << written << " bytes");
	for (long i = 0; i < bufferSize; i++)
	{
		std::cout << _buffer[i];
	}
	std::cout << std::endl;

	if (written != bufferSize)
	{
		DEBUG("Actual bytes written is not equal to the amount requested to send." << std::endl <<
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
