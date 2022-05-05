#include <stdexcept>

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include <defines.hpp>
#include "Logger.hpp"
#include <Webserv.hpp>

Webserv::Webserv(const ServerConfig& config)
	: _config(config), _listener(ServerHandler::addPollfd())
{
	setup();

	DEBUG("Created server instance on port: " << _config.port);
}

Webserv::~Webserv()
{
	close(_listener.fd);
	ServerHandler::removePollfd(_listener);

	DEBUG("Destroyed server instance on port: " << _config.port);
}

void Webserv::setup()
{
	const int socketSwitch = 1;
	struct sockaddr_in servAddr;

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(_config.port);
	servAddr.sin_addr.s_addr = INADDR_ANY;

	_listener.fd = socket(AF_INET, SOCK_STREAM, STD_TCP);
	if (_listener.fd == SYSTEM_ERR)
		throw std::runtime_error("Socket() failed");

	if (setsockopt(_listener.fd, SOL_SOCKET, SO_REUSEADDR,
				   (char *)&socketSwitch, sizeof(socketSwitch)) == SYSTEM_ERR)
		throw std::runtime_error("setsockopt() failed");

	if (bind(_listener.fd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == SYSTEM_ERR)
		throw std::runtime_error("Bind() failed");

	if (fcntl(_listener.fd, F_SETFL, O_NONBLOCK) == SYSTEM_ERR)
		throw std::runtime_error("fcntl() failed");

	if (listen(_listener.fd, _config.listen_backlog) == SYSTEM_ERR)
		throw std::runtime_error("listen() failed");

	_listener.events = POLLIN;
}

void Webserv::handleClients()
{
	size_t size = _clients.size();

	for (size_t i = 0; i < size; i++)
	{
		if (_clients[i]->handle() == false)
		{
			removeClient(i);
			--i;
			--size;
		}
	}
}

void Webserv::handleListener()
{
	if (BIT_ISSET(_listener.revents, POLLIN_BIT))
	{
		int fd = accept(_listener.fd, NULL, NULL);
		if (fd == SYSTEM_ERR)
		{
			perror("ERROR!");
		}
		if (fd != SYSTEM_ERR)
		{
			_clients.push_back(new Client(fd));
		}
	}
}

void Webserv::removeClient(int index)
{
	delete _clients[index];
	_clients.erase(_clients.begin() + index);
}

void Webserv::handle()
{
	handleListener();
	handleClients();
}
