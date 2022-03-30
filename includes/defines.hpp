#pragma once

#define SYSTEM_ERR -1
#define STD_TCP 0

#ifndef TEST
	#define private public
#endif

#define BIT_ISSET(num, pos) ((num) & (1 << pos))
#define POLLIN_BIT 0
#define POLLOUT_BIT 2
#define POLLHUP_BIT 4

typedef enum method
{
	GET,
	POST,
	DELETE
} method;

typedef enum httpStatusCode
{
	OK				= 200,
	BAD_REQUEST		= 400,
	NOT_FOUND		= 404,
	URI_TOO_LONG	= 414,
	TEAPOT			= 418,
	NOT_IMPLEMENTED	= 501
} httpStatusCode;
