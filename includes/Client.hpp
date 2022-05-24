#pragma once

#include <stdexcept>
#include <string>
#include <deque>
#include <vector>

#include <config/ServerConfig.hpp>
#include <Request.hpp>
#include <Receiver.hpp>
#include <responses/Response.hpp>
#include <Sender.hpp>

namespace Webserver
{
	class Client
	{
	public:
		Client(const ServerConfig& config, int fd);
		~Client();

		bool handle();

		struct DisconnectedException : std::exception
		{
			const char* what() const throw()
			{
				return "Client disconnected";
			}
		};

	private:
		bool checkTimeout() const;
		void hasCommunicated();
		void recvRequests();
		void sendResponses();
		void processRequests();
		Response* processValidRequest(const Request& request);

		timeval _lastCommunicated;

		std::deque<Request> _requestQueue;
		std::deque<Response *> _responseQueue;
		// std::vector<CGI*> _cgiQueue;

		int _fd;
		Receiver _receiver;
		Sender _sender;
		const ServerConfig& _serverConfig;
	};
}
