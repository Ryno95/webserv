#include <stdexcept>

#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <defines.hpp>
#include <Webserv.hpp>

// s_addr might be changed to localhost 127.0.0.1
void	Webserv::setupSocket()
{
	pollfd	listenSocket;
	struct sockaddr_in servAddr;

	listenSocket.fd = socket(AF_INET, SOCK_STREAM, STD_TCP);
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(this->_port);
	servAddr.sin_addr.s_addr = INADDR_ANY;

	if (listenSocket.fd == SYSTEM_ERR)
		throw std::runtime_error("Socket() failed");

	if (bind(listenSocket.fd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == SYSTEM_ERR)
		throw std::runtime_error("Bind() failed");

	if (fcntl(listenSocket.fd, F_SETFL, O_NONBLOCK) == SYSTEM_ERR)
		throw std::runtime_error("fcntl() failed");
	
	if (listen(listenSocket.fd, BACKLOG_AMOUNT) == SYSTEM_ERR)
		throw std::runtime_error("listen() failed");
}

Webserv::Webserv(uint port, std::string name) : _port(port), _name(name)
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
	std::cout << "Destroying Webserv instance with port : " << this->_port << std::endl;
}
