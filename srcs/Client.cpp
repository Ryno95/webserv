#include <Client.hpp>
#include <GETMethod.hpp>
#include <POSTMethod.hpp>
#include <DELETEMethod.hpp>
#include <defines.hpp>
#include <Logger.hpp>
#include <PollHandler.hpp>
#include <responses/BadStatusResponse.hpp>
#include <CGI.hpp>
#include <responses/RedirectResponse.hpp>
#include <responses/BadStatusResponse.hpp>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <iostream>

namespace Webserver
{
	Client::Client(const ServerConfig& config, int fd) : _fd(fd), _receiver(fd), _sender(fd), _serverConfig(config)
	{
		PollHandler::addPollfd(fd);
		hasCommunicated();

		DEBUG("Accepted client on fd: " << _fd);
	}

	Client::~Client()
	{
		close(_fd);
		PollHandler::removePollfd(_fd);

		DEBUG("Client disconnected: " << _fd);
	}

	/*
		Returns false when the client should be removed (disconnected)
	*/
	bool Client::handle()
	{
		if (!PollHandler::canReadOrWrite(_fd))
			return checkTimeout();

		hasCommunicated();

		try
		{
			if (PollHandler::canRead(_fd))
				recvRequests();

			processRequests();

			if (PollHandler::canWrite(_fd))
				sendResponses();
		}
		catch(const DisconnectedException& e)
		{
			return false;
		}

		return true;
	}

	void Client::recvRequests()
	{
		_receiver.handle();
		std::deque<Request> const& newRequests = _receiver.collectRequests();
		if (newRequests.size() == 0)
			return;

		PollHandler::setWriteFlag(_fd, true);

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
			Host host = Host::determine(_serverConfig, request.getHost(), request.getTarget());

			// an error occured during parsing / preparing the request, so we send the error-code back
			if (request.getStatus() != HttpStatusCodes::OK) // extra check if determine() went well;
			{
				response = new BadStatusResponse(request.getStatus(), BadRequestErrorPage);
				DEBUG("Invalid request received.");
			}
			DEBUG(request.getTarget());
			if (request.getTarget().find(".py") != std::string::npos) // host.isCGI()
			{
				DEBUG("ENTERING CGI");
				// CGI *object = new CGI();
				// _response = object->createCgiResponse();
				_cgiQueue.push_back(new CGI(request));
				// response = _cgiQueue
				// perform CGI
				// _CGIQueue.pushback(newCGI(args.....))
			}
			else
			{
				response = processValidRequest(request);
			}

			_requestQueue.pop_front();
			if (response != nullptr)
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

				_sender.setResponse(response);
				_responseQueue.pop_front();
			}
		}

		if (_sender.hasResponse()) // if we have a current response set, send that
			_sender.handle();
		else
		{ // otherwise we can deactivate POLLOUT, since there's nothing prepared for us...
			PollHandler::setWriteFlag(_fd, false);
		}
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

	bool Client::checkTimeout() const
	{
		timeval now;
		gettimeofday(&now, NULL);
		return ((now.tv_sec - _lastCommunicated.tv_sec) * 1000) + ((now.tv_usec - _lastCommunicated.tv_usec) / 1000) < TIMEOUT_MS;
	}
}
