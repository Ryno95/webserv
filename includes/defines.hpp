#pragma once

#define SYSTEM_ERR -1
#define STD_TCP 0

#ifdef TEST
	#define private public
#endif

#ifdef CI_ACCEPTANCE
	#define ROOT "/home/test_user/project/"
#else 
	#define ROOT "root/uploads/"
#endif


#pragma region CONFIG FILE

#define MAX_TARGET_LEN 50
#define BUFFER_SIZE 500
#define TIMEOUT_MS 1000

#define HTTPVERSION "HTTP/1.1"
#define HTTPVERSION_MAJOR 1
#define HTTPVERSION_MINOR 1

#pragma endregion

#define BIT_ISSET(num, pos) ((num) & (1 << pos))
#define POLLIN_BIT 0
#define POLLOUT_BIT 2
#define POLLHUP_BIT 4
