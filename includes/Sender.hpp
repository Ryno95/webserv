#pragma once

#include <Response.hpp>

/*
	Sender will get a response object as argument.
	It will use the object to determine how and how much to send.
	It will keep on sending until finished and return FINISHED.
	The client then knows that the Sender is ready to send the next Response object.
 */
class Sender
{
public:
	Sender(int fd);
	~Sender();

	bool handle(Response response);

private:
	enum state
	{
		SEND,
		FINISHED
	};

	state send();

	int _fd;
};
