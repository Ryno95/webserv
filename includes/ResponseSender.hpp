#pragma once

#include <Request.hpp>

/*
	ResponseSender will get a response object as argument.
	It will use the object to determine how and how much to send.
	It will keep on sending until finished and return FINISHED.
	The client then knows that the ResponseSender is ready to send the next Response object.
 */
class ResponseSender
{
public:
	ResponseSender(int fd);
	~ResponseSender();

	bool handle(Request request); // Returns whether the action has finished and should be collected.

private:
	enum state
	{
		SEND,
		FINISHED
	};

	state send();

	int _fd;
};
