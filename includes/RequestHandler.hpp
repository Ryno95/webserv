#pragma once

#include <AHandler.hpp>
#include <Request.hpp>

/*
	RequestHandler will handle receiving bytes and store those in a buffer.
	When the request has been received, it will parse the headers and optionally receive the body.
	When the whole request is received, it will signal with FINISHED that the request can be collected by the client handler.
 */
class RequestHandler : AHandler
{
public:
	RequestHandler();
	~RequestHandler();

	bool handle(); // Returns whether the action has finished and should be collected.

private:
	enum state
	{
		RECV_HEADER,
		RECV_BODY,
		CHECK_HEADER,
		FINISHED
	};
};
