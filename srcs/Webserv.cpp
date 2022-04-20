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

Webserv::Webserv(uint port, std::string name)
	: _port(port), _name(name)
{
	setupSocket();
}

Webserv::~Webserv()
{
	DEBUG("Destroying Webserv instance with port: " << _port);
}

void Webserv::setupSocket()
{
	const int socketSwitch = 1;
	pollfd listenSocket;
	struct sockaddr_in servAddr;

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(this->_port);
	servAddr.sin_addr.s_addr = INADDR_ANY;

	listenSocket.fd = socket(AF_INET, SOCK_STREAM, STD_TCP);
	if (listenSocket.fd == SYSTEM_ERR)
		throw std::runtime_error("Socket() failed");

	if (setsockopt(listenSocket.fd, SOL_SOCKET, SO_REUSEADDR,
				   (char *)&socketSwitch, sizeof(socketSwitch)) == SYSTEM_ERR)
		throw std::runtime_error("setsockopt() failed");

	if (bind(listenSocket.fd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == SYSTEM_ERR)
		throw std::runtime_error("Bind() failed");

	if (fcntl(listenSocket.fd, F_SETFL, O_NONBLOCK) == SYSTEM_ERR)
		throw std::runtime_error("fcntl() failed");

	if (listen(listenSocket.fd, BACKLOG_AMOUNT) == SYSTEM_ERR)
		throw std::runtime_error("listen() failed");

	listenSocket.events = POLLIN;
	this->_fds.push_back(listenSocket);
	this->_listenFd = listenSocket.fd;
}

void Webserv::handleClients()
{
	int size = _clients.size();;

	for (int i = 0; i < size; i++)
	{
		if (_clients[i].handle() == false)
		{
			removeClient(i);
			--i;
			--size;
		}
	}
}

void Webserv::handleListener()
{
	pollfd newClient;

	if (_fds[0].revents == POLLIN)
	{
		newClient.fd = accept(_listenFd, NULL, NULL);
		if (newClient.fd != SYSTEM_ERR)
		{
			newClient.events = POLLIN;
			_fds.push_back(newClient);
			_clients.push_back(Client(&_fds.back()));
			DEBUG("Accepted client on fd: " << newClient.fd);
		}
	}
}

void Webserv::removeClient(int index)
{
	DEBUG("Removing client: " << _fds[index].fd);

	close(_fds[index + 1].fd);
	_fds.erase(_fds.begin() + index + 1);
	_clients.erase(_clients.begin() + index);
}

void Webserv::run()
{
	int pollRet;

	DEBUG("Running webserver on port: " << _port);
	while (true)
	{
		pollRet = poll(&_fds.front(), _fds.size(), 100); // 100 ms is temporary
		if (pollRet == SYSTEM_ERR)
			throw std::runtime_error("poll() failed");

		handleListener();
		handleClients();
	}
}
