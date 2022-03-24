#include <Webserv.hpp>

Webserv::Webserv(uint port) : _port(port)
{
	std::cout << "Creating Webserv instance with port: " << this->_port << std::endl;
}

Webserv::~Webserv()
{
	std::cout << "Destroying Webserv instance with port : " << this->_port << std::endl;
}
