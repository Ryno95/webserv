from enum import Enum

class HttpResponseStatus:
    OK				        = 200
    CREATED			        = 201
    BAD_REQUEST		        = 400
    FORBIDDEN		        = 403
    NOT_FOUND		        = 404
    METHOD_NOT_ALLOWED		= 405
    PAYLOAD_TOO_LARGE       = 413
    URI_TOO_LONG	        = 414
    TEAPOT			        = 418
    NOT_IMPLEMENTED	        = 501

class returnStatus:
    ERROR   = 1
    SUCCESS = 0

class Colors:
    OK_GREEN    =   '\033[1;32m'
    FAILED_RED  =   '\033[1;31m'
    NATURAL     =   '\033[0m'
    BLUE        =   '\033[1;36m'

class Methods(Enum):
    GET     = 1
    POST    = 2
    DELETE  = 3
    CGI     = 4

LOCAL_HOST = "http://localhost:8080"
TEST_FILE_DIR = "acceptance_tests/PyTests/"