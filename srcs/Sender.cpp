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

bool Sender::handle(Response response)
{
	// Null bytes in body should be handled in executor
	const std::string 	resp = response.getBytes();
	const int 			size = resp.size();

	write(_fd, resp.c_str(), size);
	return true;
}
