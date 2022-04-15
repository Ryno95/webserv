#pragma once

#include <Request.hpp>
#include <deque>

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

	void handle();
	std::deque<Request> const collectRequests();

private:
	typedef enum
	{
		RECV_HEADER,
		RECV_BODY,
		ADD_REQUEST
	} state;

	void receive();
	void processHeaderRecv();
	void processBodyRecv();
	void checkHeader();

	std::string _buffer;
	std::string _recvBuffer;

	Request _newRequest;
	std::deque<Request> _readyRequests;
	int _fd;
	state _state;
	int _bodyBytesReceived;
	int _bodySize;

};
