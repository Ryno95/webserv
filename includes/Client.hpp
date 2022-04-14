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

	bool handleRequest();
	bool handleResponse();
	void handleProcessing();
	size_t getLastCommunicatedMs(timeval now) const;

	struct DisconnectedException : std::exception
	{
		const char* what() const throw()
		{
			return "Client disconnected";
		}
	};

private:
	void hasCommunicated();

	pollfd* _fd;
	timeval _lastCommunicated;

	std::deque<Request> _requests;
	std::deque<Response *> _responses;

	Receiver _receiver;
	Sender _sender;
};

