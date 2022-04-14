#include <Client.hpp>
#include <GETMethod.hpp>
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
	if (!_sender.hasResponse())
	{
		if (_responses.size() > 0)
		{
			std::cout << "Response set!" << std::endl;
			_sender.setResponse(_responses.front());
			_responses.pop_front();
		}
	}

	if (_sender.hasResponse())
		_sender.handle();
	return true;
}



bool Client::handleExecution()
{
	if (_requests.size() == 0)
		return (false);

	Response *response;
	Request const& request = _requests.front();

	switch (request.getMethod())
	{
		case GET:
			response = GETMethod(request).process();
			break;
	}

	_requests.pop_front();
	_responses.push_back(response);
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
