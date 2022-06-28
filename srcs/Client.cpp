#include <Client.hpp>
#include <methods/GETMethod.hpp>
#include <methods/POSTMethod.hpp>
#include <methods/DELETEMethod.hpp>
#include <Cgi.hpp>
#include <defines.hpp>
#include <Logger.hpp>
#include <PollHandler.hpp>
#include <TimeoutHandler.hpp>
#include <responses/Response.hpp>
#include <responses/BadResponse.hpp>
#include <responses/CgiResponse.hpp>
#include <responses/RedirectResponse.hpp>
#include <Exception.hpp>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <iostream>

namespace Webserver
{
	Client::Client(const ServerConfig& config, int fd)
		: _fd(fd), _receiver(fd), _sender(fd), _serverConfig(config), _needsRemove(false), _currentResponse(nullptr)
	{
		PollHandler::get().add(this);
		TimeoutHandler::get().add(this);
		setLastCommunicated();

		DEBUG("Accepted client on fd: " << _fd);
	}

	Client::~Client()
	{
		PollHandler::get().remove(this);
		TimeoutHandler::get().remove(this);
		close(_fd);

		DEBUG("Removed client with fd: " << _fd);
	}

	void Client::onRead()
	{
		try
		{
			setLastCommunicated();
			recvRequests();
		}
		catch(const DisconnectedException& e)
		{
			DEBUG("Client disconnected: " << _fd);
			_needsRemove = true;
		}
	}

	int Client::getFd() const
	{
		return _fd;
	}

	void Client::onWrite()
	{
		if (_needsRemove == true)
			return;

		createCurrentResponse();
		setLastCommunicated();
		if (_responseQueue.size() == 0 && !_sender.hasResponse() && _currentResponse == nullptr)
		{
			WARN("DEACTIVATING WRITE FLAG");
			PollHandler::get().setWriteEnabled(this, false);
			return ;
		}

		try
		{
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

		PollHandler::get().setWriteEnabled(this, true);

		std::deque<Request>::const_iterator first = newRequests.begin();
		std::deque<Request>::const_iterator last = newRequests.end();
		while (first != last)
		{
			_requestQueue.push_back(*first);
			++first;
		}
	}

	/*
		Process the valid request with the requested method and host.
		If no error occurs during processing, a response object is returned,
		containing the results of the request.
		If an error occurs, an InvalidRequestException is thrown. (Nothing returned, catch instead)
	*/
	Response* Client::processValidRequest(const Host& host, const Request& request)
	{
		const std::string uri(prependRoot(host.getRoot(), request.getTarget()));

		switch (host.getRouteType())
		{
			case RouteType::REDIRECT:	return new RedirectResponse(host.getRoot());
			case RouteType::CGI:		return new CgiResponse(request, host, uri);

			case RouteType::FILESERVER:
				switch (request.getMethod())
				{
					case Method::GET:		return GETMethod(request, host).process(uri);
					case Method::POST:		return POSTMethod(request, host).process(uri);
					case Method::DELETE:	return DELETEMethod(request, host).process(uri);

					default:				break;
				}
		}
		WARN("Unexpected: The requested routetype (" << host.getRouteType() << ") or method (" << request.getMethod() << ") is not supported.");
		throw InvalidRequestException(HttpStatusCodes::INTERNAL_ERROR);
	}

	Response* Client::processInvalidRequest(const Host& host, HttpStatusCode code)
	{
		return new BadResponse(host, code);
	}

	Response* Client::processRequest(const Request& request)
	{
		Host host = Host::determine(_serverConfig, request.getHost(), request.getTarget());

		try
		{
			if (!host.isMethodAllowed(request.getMethod()))
				throw InvalidRequestException(HttpStatusCodes::METHOD_NOT_ALLOWED);

			// The request we received is invalid, so we send the error-code back
			if (request.getStatus() != HttpStatusCodes::OK)
				throw InvalidRequestException(request.getStatus());

			return processValidRequest(host, request);
		}
		catch(const InvalidRequestException& e)
		{
			return processInvalidRequest(host, e.getStatus());
		}
	}

	void Client::createCurrentResponse()
	{
			// Response *response = nullptr;
		if (_requestQueue.size() == 0)
			return ;
		Request const& request = _requestQueue.front();

		if (_currentResponse == nullptr)
			_currentResponse = processRequest(request);


		if (!_currentResponse->isFinished())
			return;

		// after we created a new response, we also need to communicate "Connection: close" if the client requested that from us.
		std::string connectionValue;
		if (request.tryGetHeader(Header::Connection, connectionValue) && stringToLower(connectionValue) == "close")
			_currentResponse->addHeader(Header::Connection, "close");
		else
			_currentResponse->addHeader(Header::Connection, "keep-alive");

		_requestQueue.pop_front();
		_responseQueue.push_back(_currentResponse);
		_currentResponse = nullptr;
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

		if (_sender.hasResponse() == false && _closeAfterRespond == true)
			_needsRemove = true;
	}

	/*
		For the time functions setLastCommunicated() and checkTimeout():
			We might set current_time once per poll iteration and get that value.
			It reduces the amount of system calls!
	*/
	void Client::setLastCommunicated()
	{
		// Use TimeoutHandler::getTime() instead for optimization
		gettimeofday(&_lastCommunicated, NULL);
	}

	bool Client::needsRemove() const
	{
		return _needsRemove;
	}

	void Client::onTimeout()
	{
		_needsRemove = true;
		DEBUG("FD " << _fd << " timed-out.");
	}

	timeval Client::getLastCommunicated() const
	{
		return _lastCommunicated;
	}
}
