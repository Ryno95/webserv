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
#include <Sender.hpp>
#include <IPollable.hpp>

namespace Webserver
{
	class Webserv;
	class Client : public IPollable
	{
	public:
		Client(const ServerConfig& config, int fd);
		~Client();

		void readHandler();
		void writeHandler();
		bool needsRemove() const;
		void checkTimeout(timeval now);

		struct DisconnectedException : std::exception
		{
			const char* what() const throw()
			{
				return "Client disconnected";
			}
		};

	private:
		void hasCommunicated();
		void recvRequests();
		void sendResponses();
		void processRequests();
		Response* processValidRequest(const Request& request);

		timeval _lastCommunicated;

		std::deque<Request> _requestQueue;
		std::deque<Response *> _responseQueue;

		int _fd;
		Receiver _receiver;
		Sender _sender;
		const ServerConfig& _serverConfig;
		bool _closeAfterRespond;
		bool _needsRemove;
	};
}
