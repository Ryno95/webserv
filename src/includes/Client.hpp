#pragma once

#include <stdexcept>
#include <string>
#include <deque>
#include <vector>

#include <Webserv.hpp>
#include <config/ServerConfig.hpp>
#include <Request.hpp>
#include <Receiver.hpp>
#include <responses/Response.hpp>
#include <cgi/Cgi.hpp>
#include <Sender.hpp>
#include <IPollable.hpp>
#include <ITimeoutable.hpp>

namespace Webserver
{
	class Webserv;
	class Client : public IPollable, public ITimeoutable
	{
	public:
		Client(const ServerConfig& config, int fd);
		~Client();

		void onTimeout();
		timeval getLastCommunicated() const;
		int getFd() const;
		void onRead();
		void onWrite();
		bool needsRemove() const;

		struct DisconnectedException : std::exception
		{
			const char* what() const throw()
			{
				return "Client disconnected";
			}
		};

	private:
		void setLastCommunicated();
		void recvRequests();
		void sendResponses();
		void startProcessingRequest();
		void addResponseToQueue();
		Response* processRequest(const Request& request);
		Response* processValidRequest(const Host& host, const Request& request);
		Response* processInvalidRequest(const Host& host, HttpStatusCode code);

		timeval _lastCommunicated;

		std::deque<Request> _requestQueue;
		std::deque<Response *> _responseQueue;

		int _fd;
		Receiver _receiver;
		Sender _sender;
		const ServerConfig& _serverConfig;
		bool _closeAfterRespond;
		bool _needsRemove;
		Response* _currentlyProcessing;
	};
}
