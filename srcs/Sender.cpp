#include <Sender.hpp>
#include <defines.hpp>

#include <iostream>
#include <string>

#include <unistd.h>

Sender::Sender(int fd) : _fd(fd), _buffer(new char[BUFFER_SIZE])
{
}

Sender::~Sender()
{
	delete[] _buffer;
}

void Sender::setDataStream()
{
	switch (_currentState)
	{
		case SEND_HEADER:
			_dataStream = _response.getHeaderStream();
			break;

		case SEND_BODY:
			_dataStream = _response.getBodyStream();
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
		if (_currentState == SEND_BODY)
			std::cout << "[DEBUG] " << "Data stream for Body is nullptr!" << std::endl;
		else
			std::cout << "[DEBUG] " << "THIS SHOULD NOT OCCUR!" << std::endl;
		return 0;
	}

	_dataStream->read(_buffer + bufferSize, BUFFER_SIZE - bufferSize);
	return _dataStream->gcount();
}

void Sender::handle()
{
	long bufferSize = 0;

	while (bufferSize < BUFFER_SIZE && _currentState != FINISHED)
	{
		if (_dataStream == nullptr)
			setDataStream();
		bufferSize += fillBuffer(bufferSize);
		_currentState++;
	}

	if (bufferSize > BUFFER_SIZE)
		throw std::runtime_error("UNEXPECTED AMOUNT OF BYTES STORED IN THE BUFFER! (Sender.cpp)"); // DEBUG LINE

	write(_fd, _buffer, bufferSize);
}

bool Sender::hasResponse() const
{
	return _currentState != FINISHED;
}

void Sender::setResponse(Response response)
{
	_response = response;
	_currentState = START;
}
