#include <ResponseSender.hpp>
#include <defines.hpp>

#include <iostream>
#include <string>

#include <unistd.h>

ResponseSender::ResponseSender(int fd) : _fd(fd)
{
}

ResponseSender::~ResponseSender()
{
}

bool ResponseSender::handle(Request request)
{
	std::string buffer = HTTPVERSION;
	buffer += " ";
	buffer += std::to_string(request.getStatus().first);
	buffer += " ";
	buffer += request.getStatus().second;
	buffer += "\r\ncontent-length: 17\r\n\r\nSERVER GOES BRRRR";

	std::cout << "Sending to " << _fd << ": " << std::endl << buffer << std::endl;

	write(_fd, buffer.c_str(), buffer.size());
	return true;
}
