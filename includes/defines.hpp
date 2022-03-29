#pragma once

#ifdef TEST
	#define private public
#endif

#define SYSTEM_ERR -1
#define STD_TCP 0

// Defines to be contained in config file

#define MAX_TARGET_LEN 50
#define BUFFER_SIZE 9

// End of region config file

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
