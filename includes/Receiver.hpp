#pragma once

#include <Request.hpp>
#include <deque>

namespace Webserver
{
	/*
		Receiver will handle receiving bytes and store those in a buffer.
		When the request has been received, it will parse the headers and optionally receive the body.
		Fully received requests are stored and can be retrieved by calling Receiver::collectRequests().
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
		void processBodyContentLength();
		void processBodyChunked();
		void checkHeader();

		bool requestHasContentLength();
		bool requestHasChunkedEncoding();

		std::string _buffer;
		std::string _recvBuffer;

		Request _newRequest;
		std::deque<Request> _readyRequests;
		int _fd;
		state _state;
		uint _bodyBytesReceived;
		uint _bodySize;
		void (Receiver::*_processBodyFunction)(void);

	};
}
