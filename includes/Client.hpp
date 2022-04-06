#pragma once

#include <stdexcept>
#include <string>
#include <deque>
#include <vector>

#include <Request.hpp>
#include <RequestReceiver.hpp>
#include <Response.hpp>
#include <ResponseSender.hpp>

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
	int _fd;

	// std::deque<Request> _requests;
	// std::deque<Response> _responses;

	RequestReceiver _requestReceiver;
	ResponseSender _responseSender;
};
