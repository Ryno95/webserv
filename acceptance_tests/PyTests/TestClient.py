
import time
import requests
from requests import Session
from enum import Enum

class Colors:
    OK_GREEN =   '\033[92m'
    FAILED_RED =   '\033[91m'
    NATURAL =   '\033[0m'

OK				= 200
BAD_REQUEST		= 400
NOT_FOUND		= 404
URI_TOO_LONG	= 414
TEAPOT			= 418
NOT_IMPLEMENTED	= 501

ERROR = 1
SUCCESS = 0
localhost = "http://localhost:8080"
EXIT_CODE = 0

def compareExpectedPositiveResult(response: requests):
    if response.status_code != OK:
        print(f"{Colors.FAILED_RED}[KO] {Colors.NATURAL} Get request on {response.url}")
        return ERROR
    else:
        print(f"{Colors.OK_GREEN}[OK] {Colors.NATURAL} Get request on {response.url}")
        return SUCCESS

EXIT_CODE += compareExpectedPositiveResult(requests.get(localhost))

# # sleep so that the exit code is that of the python script and not the server
time.sleep(1)

exit(EXIT_CODE)



