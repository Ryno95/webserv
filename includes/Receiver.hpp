#pragma once

#include <Request.hpp>

/*
	Receiver will handle receiving bytes and store those in a buffer.
	When the request has been received, it will parse the headers and optionally receive the body.
	When the whole request is received, it will signal with FINISHED that the request can be collected by the client handler.
 */
class Receiver
{
public:
	Receiver(int fd);
	~Receiver();

	bool handle(); // Returns whether the action has finished and should be collected.
	Request getRequest();

private:
	typedef enum
	{
		RECV_HEADER,
		RECV_BODY,
		CHECK_HEADER,
		FINISHED
	} state;

	void receive();
	void processHeaderRecv();
	void processBodyRecv();

	std::string _buffer;
	Request _request;
	int _fd;
	state _state;
	int _bodyBytesReceived;
	int _bodySize;

};
