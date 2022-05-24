#include <Client.hpp>
#include <GETMethod.hpp>
#include <POSTMethod.hpp>
#include <DELETEMethod.hpp>
#include <defines.hpp>
#include <Logger.hpp>
#include <PollHandler.hpp>
#include <responses/RedirectResponse.hpp>
#include <responses/BadStatusResponse.hpp>
#include <responses/OkStatusResponse.hpp>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <iostream>

namespace Webserver
{
	Client::Client(const ServerConfig& config, int fd) : _fd(fd), _receiver(fd), _sender(fd), _serverConfig(config), _needsRemove(false)
	{
		PollHandler::add(fd, this);
		hasCommunicated();

		DEBUG("Accepted client on fd: " << _fd);
	}

	Client::~Client()
	{
		close(_fd);
		PollHandler::remove(_fd);
		DEBUG("Removed client with fd: " << _fd);
	}

	void Client::readHandler()
	{
		try
		{
			hasCommunicated();
			recvRequests();
			processRequests();
		}
		catch(const DisconnectedException& e)
		{
			DEBUG("Client disconnected: " << _fd);
			_needsRemove = true;
		}
	}

	void Client::writeHandler()
	{
		if (_needsRemove == true)
			return;

		try
		{
			hasCommunicated();
			sendResponses();
		}
		catch(const DisconnectedException& e)
		{
			DEBUG("Client disconnected: " << _fd);
			_needsRemove = true;
		}
	}

	void Client::recvRequests()
	{
		_receiver.handle();
		std::deque<Request> const& newRequests = _receiver.collectRequests();
		if (newRequests.size() == 0)
			return;

		PollHandler::setWriteEnabled(_fd, true);

		std::deque<Request>::const_iterator first = newRequests.begin();
		std::deque<Request>::const_iterator last = newRequests.end();
		while (first != last)
		{
			_requestQueue.push_back(*first);
			++first;
		}
	}

	Response* Client::processValidRequest(const Request& request)
	{
		Host host = Host::determine(_serverConfig, request.getHost(), request.getTarget());
		DEBUG("Using config: " << host.getName());

		if (host.isRedirect())
		{
			DEBUG("Redirection encountered.");
			return new RedirectResponse(host.getRoot());
		}

		if (host.isCgi())
		{
			DEBUG("CGI triggered.");
			return new OkStatusResponse(HttpStatusCodes::OK);
		}

		switch (request.getMethod())
		{
			case Method::GET:		return GETMethod(request, host).process();
			case Method::POST:		return POSTMethod(request, host).process();
			case Method::DELETE:	return DELETEMethod(request, host).process();

			default:
				WARN("INVALID method still continued processing, which is not expected to occur.");
		}
		return nullptr;
	}

	void Client::processRequests()
	{
		while (_requestQueue.size() > 0)
		{
			Response *response = nullptr;
			Request const& request = _requestQueue.front();

			// an error occured during parsing / preparing the request, so we send the error-code back
			if (request.getStatus() != HttpStatusCodes::OK)
				response = new BadStatusResponse(request.getStatus());
			else
				response = processValidRequest(request);

			// after we created a new response, we also need to communicate "Connection: close" if the client requested that from us.
			std::string connectionValue;
			if (request.tryGetHeader(Header::Connection, connectionValue) && stringToLower(connectionValue) == "close")
				response->addHeader(Header::Connection, "close");
			else
				response->addHeader(Header::Connection, "keep-alive");

			_requestQueue.pop_front();
			if (response != nullptr) // If we ALWAYS want to respond, we could send BAD_REQUEST
				_responseQueue.push_back(response);
		}
	}

	void Client::sendResponses()
	{
		if (!_sender.hasResponse()) // Is the sender still sending a response?
		{
			if (_responseQueue.size() > 0) // If we have some queued, pop the front
			{
				Response* response = _responseQueue.front();

				// Check connection header to know if we should close after sending this response
				std::string connectionValue;
				if (response->tryGetHeader(Header::Connection, connectionValue) && stringToLower(connectionValue) == "close")
					_closeAfterRespond = true;
				else
					_closeAfterRespond = false;

				_sender.setResponse(response);
				_responseQueue.pop_front();
			}
		}

		if (_sender.hasResponse()) // if we have a current response set, send that
			_sender.handle();
		else
		{ // otherwise we can deactivate POLLOUT, since there's nothing prepared for us...
			PollHandler::setWriteEnabled(_fd, false);
		}

		if (_sender.hasResponse() == false && _closeAfterRespond == true)
			_needsRemove = true;
	}

	/*
		For the time functions hasCommunicated() and checkTimeout():
			We might set current_time once per poll iteration and get that value.
			It reduces the amount of system calls!
	*/
	void Client::hasCommunicated()
	{
		gettimeofday(&_lastCommunicated, NULL);
	}

	bool Client::needsRemove() const
	{
		return _needsRemove;
	}

	void Client::checkTimeout(timeval now)
	{
		if (((now.tv_sec - _lastCommunicated.tv_sec) * 1000) + ((now.tv_usec - _lastCommunicated.tv_usec) / 1000) >= TIMEOUT_MS)
		{
			_needsRemove = true;
			DEBUG("Connection timed-out fd: " << _fd);
		}
	}
}
