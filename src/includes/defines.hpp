#pragma once

#define SYSTEM_ERR -1
#define STD_TCP 0

#ifdef TEST
	#define private public
	#define protected public
#endif


#define ROOT "./root/uploads/"

#define ERROR_PAGES_DIR "root/error_pages/"

#define DEBUG_ENABLED		true
#define LOGGING_ENABLED		true
#define LOGFILE				"logs.log"

#pragma region CONFIG FILE

#define MAX_TARGET_LEN 50
#define TIMEOUT_MS 1000
#define BUFFERSIZE 1024

#define HTTPVERSION "HTTP/1.1"
#define HTTPVERSION_MAJOR 1
#define HTTPVERSION_MINOR 1

#define BODY_SIZE_LIMIT 10240
#define SERVER_NAME "Webserv"
#define DEFAULT_ERROR_PAGE "root/error.html"


#pragma endregion

#define BIT_ISSET(num, pos) ((num) & (1 << pos))
#define POLLIN_BIT 0
#define POLLOUT_BIT 2
#define POLLHUP_BIT 4
