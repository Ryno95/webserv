#include <Client.hpp>
#include <defines.hpp>

#include <unistd.h>
#include <sys/socket.h>

#include <iostream>

Client::Client(int fd) : _fd(fd), _receiver(fd), _sender(fd)
{
}

Client::~Client()
{
}

/*
	Returns false when the client should be removed (disconnected)
*/
bool Client::handleRequest()
{
	try
	{
		_receiver.handle();
		std::deque<Request> const& newRequests = _receiver.collectRequests();
		if (newRequests.size() == 0)
			return true;
		std::deque<Request>::const_iterator first = newRequests.begin();
		std::deque<Request>::const_iterator last = newRequests.end();
		while (first != last)
		{
			_requests.push_back(*first);
			++first;
			std::cout << "PUSHED ON THE CLIENT QUEUE" << std::endl;
		}
	}
	catch(const DisconnectedException& e)
	{
		std::cerr << e.what() << '\n';
		return false;
	}
	return true;
}

/*
	Returns false when the client should be removed (disconnected)
*/
bool Client::handleResponse()
{
	while (_requests.size() > 0)
	{
		std::cout << "Requests ready for us: " << _requests.size() << std::endl;
		Request request = _requests.front();
		_requests.pop_front();
		_sender.handle(request);
	}
	return true;
}
