#include <Client.hpp>
#include <defines.hpp>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <iostream>

Client::Client(int fd) : _fd(fd), _receiver(fd), _sender(fd)
{
	hasCommunicated();
}

Client::~Client()
{
}

/*
	Returns false when the client should be removed (disconnected)
*/
bool Client::handleRequest()
{
	hasCommunicated();
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

/*
	Maybe give this function a [timeval now] argument and gettimeofday() from Webserv.cpp instead,
	so we don't call gettimeofday for each client in the iteration
*/
void Client::hasCommunicated()
{
	gettimeofday(&_lastCommunicated, nullptr);
}

size_t Client::getLastCommunicatedMs(timeval now) const
{
	return ((now.tv_sec - _lastCommunicated.tv_sec) * 1000) + ((now.tv_usec - _lastCommunicated.tv_usec) / 1000);
}
