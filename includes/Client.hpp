#pragma once

#include <poll.h>

#include <stdexcept>
#include <string>
#include <deque>
#include <vector>

#include <Request.hpp>
#include <Receiver.hpp>
#include <Response.hpp>
#include <Sender.hpp>

class Client
{

public:
	Client(pollfd* fd);
	~Client();

	bool handle();

	struct DisconnectedException : std::exception
	{
		const char* what() const throw()
		{
			return "Client disconnected";
		}
	};

private:
	bool checkTimeout() const;
	void hasCommunicated();
	void handleRequest();
	void handleResponse();
	void handleProcessing();

	pollfd* _fd;
	timeval _lastCommunicated;

	std::deque<Request> _requests;
	std::deque<Response *> _responses;

	Receiver _receiver;
	Sender _sender;
};

