#include <Sender.hpp>
#include <defines.hpp>

#include <iostream>
#include <string>

#include <unistd.h>

#include <stack>

Sender::Sender(int fd) : _fd(fd), _currentState(FINISHED), _response(nullptr), _buffer(new char[BUFFER_SIZE])
{
}

Sender::Sender(Sender const& rhs) : _currentState(FINISHED), _response(nullptr), _buffer(new char[BUFFER_SIZE])
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
	std::cout << "filling buffer" <<std::endl;
	if (_dataStream == nullptr)
	{
		if (_currentState == SEND_BODY)
			std::cout << "[DEBUG] " << "Data stream for Body is nullptr!" << std::endl;
		else
			std::cout << "[DEBUG] " << "THIS SHOULD NOT OCCUR!" << std::endl;
		return 0;
	}

	_dataStream->read(_buffer + bufferSize, BUFFER_SIZE - bufferSize);
	std::cout << "Read: " << _dataStream->gcount() <<std::endl;
	return _dataStream->gcount();
}

void Sender::handle()
{
	std::cout << "Handling!" << std::endl;
	long bufferSize = 0;

	while (bufferSize < BUFFER_SIZE && _currentState != FINISHED)
	{
		if (_dataStream == nullptr)
			setDataStream();
		bufferSize += fillBuffer(bufferSize);
		if (bufferSize < BUFFER_SIZE)
		{
			_currentState++;
			_dataStream = nullptr;
			std::cout << "State incremented to: " << _currentState << std::endl;
		}
	}

	if (_currentState == FINISHED)
		deleteResponse();

	if (bufferSize == 0)
	{
		std::cout << "[DEBUG] No bytes to send!" << std::endl; // DEBUG LINE
		return;
	}

	if (bufferSize > BUFFER_SIZE)
		throw std::runtime_error("UNEXPECTED AMOUNT OF BYTES STORED IN THE BUFFER! (Sender.cpp)"); // DEBUG LINE

	ssize_t written;
	written = write(_fd, _buffer, bufferSize);

	std::cout << "Sent " << written << " bytes" <<std::endl;
	for (long i = 0; i < bufferSize; i++)
	{
		std::cout << _buffer[i];
	}
	std::cout << std::endl;

	if (written != bufferSize)
	{
		std::cout << "Actual bytes written is not equal to the amount requested to send." << std::endl; // DEBUG SECTION
		std::cout << "There is no implementation to catch this issue yet." << std::endl;
		std::cout << "Requested: " << bufferSize << " written: " << written << std::endl;
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
