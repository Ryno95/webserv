
class HttpResponseStatus:
    OK				= 200
    CREATED			= 201
    BAD_REQUEST		= 400
    NOT_FOUND		= 404
    URI_TOO_LONG	= 414
    TEAPOT			= 418
    NOT_IMPLEMENTED	= 501

class returnStatus:
    ERROR = 1
    SUCCESS = 0

class Colors:
    OK_GREEN =   '\033[1;32m'
    FAILED_RED =   '\033[1;31m'
    NATURAL =   '\033[0m'
    BLUE =   '\033[1;36m'

class Methods:
    GET = 1
    POST = 2
    DELETE = 3
