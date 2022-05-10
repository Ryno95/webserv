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
	Client(int fd);
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

	timeval _lastCommunicated;

	std::deque<Request> _requests;
	std::deque<Response *> _responses;

	int _fd;
	Receiver _receiver;
	Sender _sender;
};
