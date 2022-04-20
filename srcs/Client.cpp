#include <Client.hpp>
#include <GETMethod.hpp>
#include <POSTMethod.hpp>
#include <defines.hpp>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <iostream>

Client::Client(pollfd* fd) : _fd(fd), _receiver(fd->fd), _sender(fd->fd)
{
	hasCommunicated();
}

Client::~Client()
{
}

/*
	Returns false when the client should be removed (disconnected)
*/
bool Client::handle()
{
	if (_fd->revents == 0)
		return checkTimeout();

	hasCommunicated();

	try
	{
		if (BIT_ISSET(_fd->revents, POLLIN_BIT))
			handleRequest();

		handleProcessing();

		if (BIT_ISSET(_fd->revents, POLLOUT_BIT))
			handleResponse();
	}
	catch(const DisconnectedException& e)
	{
		std::cerr << e.what() << '\n';
		return false;
	}

	return true;
}

void Client::handleRequest()
{
	_receiver.handle();
	std::deque<Request> const& newRequests = _receiver.collectRequests();
	if (newRequests.size() == 0)
		return;

	_fd->events = POLLIN | POLLOUT;

	std::deque<Request>::const_iterator first = newRequests.begin();
	std::deque<Request>::const_iterator last = newRequests.end();
	while (first != last)
	{
		_requests.push_back(*first);
		++first;
	}
}

void Client::handleProcessing()
{
	if (_requests.size() == 0)
		return;

	Response *response;
	Request const& request = _requests.front();

	switch (request.getMethod())
	{
		case GET:
			std::cout << "Entering GET method!\n";
			response = GETMethod(request).process();
			break;
		case POST:
			std::cout << "Entering POST method!\n";
			response = POSTMethod(request).process();
			break;
	}

	_requests.pop_front();
	_responses.push_back(response);

	std::cout << "POLLOUT activated" << std::endl;
}

void Client::handleResponse()
{
	if (!_sender.hasResponse()) // set new response object as current response to send
	{
		if (_responses.size() > 0)
		{
			std::cout << "Response set!" << std::endl;
			_sender.setResponse(_responses.front());
			_responses.pop_front();
		}
	}

	if (_sender.hasResponse()) // if we have a current response set, send that
		_sender.handle();
	else
	{ // otherwise we can deactivate POLLOUT, since there's nothing prepared for us...
		_fd->events = POLLIN;
		std::cout << "POLLOUT deactivated" << std::endl;
	}
}

/*
	For the time functions hasCommunicated() and checkTimeout():
		We might set a "now" time in Webserv.cpp or somewhere else, after poll, and get that value.
		It reduces the amount of system calls!
*/
void Client::hasCommunicated()
{
	gettimeofday(&_lastCommunicated, nullptr);
}

bool Client::checkTimeout() const
{
	timeval now;
	gettimeofday(&now, NULL);
	return ((now.tv_sec - _lastCommunicated.tv_sec) * 1000) + ((now.tv_usec - _lastCommunicated.tv_usec) / 1000) < TIMEOUT_MS;
}
