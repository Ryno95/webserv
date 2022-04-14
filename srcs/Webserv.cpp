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
#include <Webserv.hpp>

Webserv::Webserv(uint port, std::string name)
	: _port(port), _name(name)
{
	try
	{
		setupSocket();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cout << "Creating Webserv instance with port: " << this->_port << std::endl;
}

Webserv::~Webserv()
{
	std::cout << "Destroying Webserv instance with port : " << this->_port << std::endl;
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
	int fdSize;

	fdSize = _fds.size();

	for (int i = 1; i < fdSize; ++i) // i = 1, because we don't need to check the listening socket
	{
		if (_fds[i].revents == 0)
			continue;

		if (BIT_ISSET(this->_fds[i].revents, POLLIN_BIT))
		{
			if (_clients[i - 1].handleRequest() == false)
			{
				removeClient(i);
				--i;
				--fdSize;
				continue;
			}
		}

		if (BIT_ISSET(this->_fds[i].revents, POLLOUT_BIT))
		{
			_clients[i - 1].handleProcessing();
			if (_clients[i - 1].handleResponse() == false)
			{
				removeClient(i);
				--i;
				--fdSize;
				continue;
			}
		}
	}
}

void Webserv::handleListener()
{
	pollfd newClient;

	if (_fds[0].revents == POLLIN)
	{
		std::cout << "Accepting new client..." << std::endl;
		newClient.fd = accept(_listenFd, NULL, NULL);
		if (newClient.fd != SYSTEM_ERR)
		{
			newClient.events = POLLIN;
			_fds.push_back(newClient);
			_clients.push_back(Client(&_fds.back()));
			std::cout << "Accepted client on fd: " << newClient.fd << std::endl;
		}
		else
			std::cout << "Nothing accepted." << std::endl;
	}
}

void Webserv::removeClient(int index)
{
	std::cout << "Removing client: " << _fds[index].fd << std::endl;

	close(_fds[index].fd);
	_fds.erase(_fds.begin() + index);
	_clients.erase(_clients.begin() + index - 1);
}

void Webserv::handleTimeout()
{
	timeval now;
	gettimeofday(&now, nullptr);

	for (size_t i = 0; i < _clients.size(); ++i)
	{
		if (_clients[i].getLastCommunicatedMs(now) >= TIMEOUT_MS)
		{
			std::cout << "Client on fd " << _fds[i + 1].fd << " timed-out." << std::endl;
			removeClient(i + 1);
			--i;
		}
	}
}

void Webserv::run()
{
	int pollRet;

	while (true)
	{
		pollRet = poll(&_fds.front(), _fds.size(), 100); // 100 ms is temporary
		if (pollRet == SYSTEM_ERR)
			throw std::runtime_error("poll() failed");

		handleListener();
		handleClients();
		handleTimeout();
	}
}
