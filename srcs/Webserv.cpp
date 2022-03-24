#include <stdexcept>

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/stat.h>
#include <sys/socket.h>
 #include <unistd.h>

#include <defines.hpp>
#include <Webserv.hpp>

Webserv::Webserv(uint port, std::string name)
	: _port(port), _buffer(new char[BUFFER_SIZE]), _name(name)
{
	try {
		setupSocket();
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	std::cout << "Creating Webserv instance with port: " << this->_port << std::endl;
}

Webserv::~Webserv()
{
	delete [] _buffer;
	std::cout << "Destroying Webserv instance with port : " << this->_port << std::endl;
}

void	Webserv::setupSocket()
{
	const int			socketSwitch = 1;
	pollfd				listenSocket;
	struct sockaddr_in	servAddr;

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(this->_port);
	servAddr.sin_addr.s_addr = INADDR_ANY;


	listenSocket.fd = socket(AF_INET, SOCK_STREAM, STD_TCP);
	if (listenSocket.fd == SYSTEM_ERR)
		throw std::runtime_error("Socket() failed");

	if (setsockopt(listenSocket.fd, SOL_SOCKET, SO_REUSEADDR,
		(char *)& socketSwitch, sizeof(socketSwitch)) == SYSTEM_ERR)
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

void	Webserv::run()
{
	int		pollRet;
	pollfd	newClient;
	int		fdSize;
	int		receivedBytes;

	while(true)
	{
		pollRet = poll(&this->_fds.front(), this->_fds.size(), - 1);
		std::cout << "LOOPING POLLY: " << pollRet << std::endl;
		if (pollRet == SYSTEM_ERR)
			throw std::runtime_error("poll() failed");
		else if (pollRet == 0)
		{
			perror("poll() timed out, program ends");
			break;
		}
		fdSize = this->_fds.size();
		for (int i = 0; i < fdSize; ++i)
		{
			std::cout << "Revents : " << this->_fds[i].revents << std::endl;
			if (this->_fds[i].revents == 0)
				continue ;
			else if (this->_fds[i].revents == DISCONNECT)
			{
				std::cout << "Removing client\n";
				close(this->_fds[i].fd);
				this->_fds.erase(this->_fds.begin() + i);
				--i;
				--fdSize;
			}
			std::cout << "Size: " << fdSize << std::endl; 
			if (this->_fds[i].revents == POLLIN)
			{
				if (this->_fds[i].fd == this->_listenFd)
				{
					// Accepting a new client, replace with class methods
					std::cout << "Accepting new client\n";
					newClient.fd = accept(this->_listenFd, NULL, NULL);
					if (newClient.fd != SYSTEM_ERR)
					{
						newClient.events = POLLIN;
						this->_fds.push_back(newClient);
						++fdSize;
					}
				}
			}
			bzero(_buffer, BUFFER_SIZE);
			receivedBytes = recv(this->_fds[i].fd, _buffer, BUFFER_SIZE - 1, 0);
			std::cout << "RB: " << receivedBytes << std::endl;
			if (receivedBytes == 0)
			{
				std::cout << "Removing client\n";
				close(this->_fds[i].fd);
				this->_fds.erase(this->_fds.begin() + i);
				--i;
				--fdSize;
			}
			if (receivedBytes != SYSTEM_ERR)
				std::cout << "Received: \n" << _buffer << std::endl;
		}
		
	}
}

