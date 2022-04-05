#pragma once

#define SYSTEM_ERR -1
#define STD_TCP 0

#ifdef TEST
	#define private public
#endif


#pragma region CONFIG FILE

#define MAX_TARGET_LEN 50

#define HTTPVERSION "HTTP/1.1"
#define HTTPVERSION_MAJOR 1
#define HTTPVERSION_MINOR 1

#pragma endregion

#define BIT_ISSET(num, pos) ((num) & (1 << pos))
#define POLLIN_BIT 0
#define POLLOUT_BIT 2
#define POLLHUP_BIT 4
