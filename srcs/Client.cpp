#include <Client.hpp>
#include <GETMethod.hpp>
#include <POSTMethod.hpp>
#include <defines.hpp>
#include <Logger.hpp>
#include <PollHandler.hpp>
#include <responses/BadStatusResponse.hpp>
#include <CGI.hpp>
#include <responses/RedirectResponse.hpp>

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

	void Client::processRequests()
	{
		while (_requestQueue.size() > 0)
		{
			Response *response = nullptr;
			Request const& request = _requestQueue.front();

			// an error occured during parsing / preparing the request, so we send the error-code back
			if (request.getStatus() != HttpStatusCodes::OK)
			{
				response = new BadStatusResponse(request.getStatus(), BadRequestErrorPage);
				DEBUG("Invalid request received.");
			}
			if (true) // isCGI()
			{
				_CGIQueue.push_back(new CGI(request, _router));
				// perform CGI
				// _CGIQueue.pushback(newCGI(args.....))
			}
			else
			{
				// Determine which host to use
				Host host = Host::determine(_serverConfig, request.getHost(), request.getTarget());
				if (host.isRedirect())
				{
					response = new RedirectResponse(request.getTarget());
					DEBUG("Redirection encountered.");
				}
				else
				{
					DEBUG("Using config: " << host.getName());

					switch (request.getMethod())
					{
						case Method::GET:
							DEBUG("Entering GET method!");
							response = GETMethod(request, host).process();
							break;
						case Method::POST:
							DEBUG("Entering POST method!");
							response = POSTMethod(request, host).process();
							break;
						case Method::DELETE:
							WARN("DELETE is not yet implemented!");
							break;
						case Method::INVALID:
							WARN("INVALID method still continued processing, which is not expected to occur.");
							break;
					}
				}
			}

			_requestQueue.pop_front();
			if (response != nullptr)
				_responseQueue.push_back(response);
		}
	}

	void Client::sendResponses()
	{
		if (!_sender.hasResponse()) // set new response object as current response to send
		{
			if (_responseQueue.size() > 0)
			{
				_sender.setResponse(_responseQueue.front());
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
			We might set a "now" time in Webserv.cpp or somewhere else, after poll, and get that value.
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
