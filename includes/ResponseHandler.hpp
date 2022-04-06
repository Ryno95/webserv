#pragma once

#include <AHandler.hpp>

/*
	ResponseHandler will get a response object as argument.
	It will use the object to determine how and how much to send.
	It will keep on sending until finished and return FINISHED.
	The client then knows that the ResponseHandler is ready to send the next Response object.
 */
class ResponseHandler
{
public:
	ResponseHandler();
	~ResponseHandler();

	bool handle(); // Returns whether the action has finished and should be collected.

private:
	enum state
	{
		SEND,
		FINISHED
	};
};
