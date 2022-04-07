#include <Sender.hpp>
#include <defines.hpp>

#include <iostream>
#include <string>

#include <unistd.h>

Sender::Sender(int fd) : _fd(fd)
{
}

Sender::~Sender()
{
}

bool Sender::handle(Request request)
{
	std::string buffer = HTTPVERSION;
	buffer += " ";
	buffer += std::to_string(request.getStatus().first);
	buffer += " ";
	buffer += request.getStatus().second;
	buffer += "\r\ncontent-length: ";
	buffer += std::to_string(request.getTarget().size());
	buffer += "\r\n\r\n";
	buffer += request.getTarget();

	std::cout << "Sending to " << _fd << ": " << std::endl << buffer << std::endl << std::endl;

	write(_fd, buffer.c_str(), buffer.size());
	return true;
}
