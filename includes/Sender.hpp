#pragma once

#include <Response.hpp>

#include <iostream>

/*
	Sender will send the currently assignd Response object.
	Before calling handle, first make sure that a Response object is set using hasResponse and setResponse.
*/
class Sender
{
public:
	Sender(int fd);
	Sender(Sender const& rhs);
	~Sender();
	Sender& operator=(Sender const& rhs);

	void handle();
	bool hasResponse() const;
	void setResponse(Response* response);

private:
	enum state
	{
		START,
		SEND_HEADER,
		SEND_BODY,
		FINISHED
	};

	void setDataStream();
	long fillBuffer(long bufferSize);
	void deleteResponse();

	int _fd;
	int _currentState;
	Response* _response;
	std::istream *_dataStream;
	char* _buffer;
};
