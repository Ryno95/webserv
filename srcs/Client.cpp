#include <Client.hpp>
#include <GETMethod.hpp>
#include <POSTMethod.hpp>
#include <defines.hpp>
#include <Logger.hpp>
#include <ServerHandler.hpp>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <iostream>

Client::Client(int fd) : _receiver(fd), _sender(fd), _pollfd(ServerHandler::addPollfd())
{
	_pollfd.fd = fd;
	_pollfd.events = POLLIN;
	hasCommunicated();

	DEBUG("Accepted client on fd: " << fd);
}

Client::~Client()
{
	close(_pollfd.fd);
	ServerHandler::removePollfd(_pollfd);

	DEBUG("Client disconnected: " << _pollfd.fd);
}

/*
	Returns false when the client should be removed (disconnected)
*/
bool Client::handle()
{
	if (_pollfd.revents == 0)
		return checkTimeout();

	hasCommunicated();

	try
	{
		if (BIT_ISSET(_pollfd.revents, POLLIN_BIT))
			handleRequest();

		handleProcessing();

		if (BIT_ISSET(_pollfd.revents, POLLOUT_BIT))
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

	_pollfd.events = POLLIN | POLLOUT;

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
		case Method::GET:
			DEBUG("Entering GET method!");
			response = GETMethod(request).process();
			break;
		case Method::POST:
			DEBUG("Entering POST method!");
			response = POSTMethod(request).process();
			break;
		case Method::DELETE:
			WARN("DELETE is not yet implemented!");
			break;
	}

	_requests.pop_front();
	_responses.push_back(response);
}

void Client::handleResponse()
{
	if (!_sender.hasResponse()) // set new response object as current response to send
	{
		if (_responses.size() > 0)
		{
			_sender.setResponse(_responses.front());
			_responses.pop_front();
		}
	}

	if (_sender.hasResponse()) // if we have a current response set, send that
		_sender.handle();
	else
	{ // otherwise we can deactivate POLLOUT, since there's nothing prepared for us...
		_pollfd.events = POLLIN;
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
