#pragma once

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

	bool handleRequest();
	bool handleResponse();

	struct DisconnectedException : std::exception
	{
		const char* what() const throw()
		{
			return "Client disconnected";
		}
	};

private:
	void hasCommunicated();

	int _fd;
	timeval _lastCommunicated;

	std::deque<Request> _requests;
	// std::deque<Response> _responses;

	Receiver _receiver;
	Sender _sender;
};
