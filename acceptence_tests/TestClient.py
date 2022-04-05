import time
import requests
from requests import Session
from enum import Enum

class Colors:
    OKGREEN =   '\033[92m'
    FAILRED =   '\033[91m'
    NATURAL =   '\033[0m'

OK				= 200
BAD_REQUEST		= 400
NOT_FOUND		= 404
URI_TOO_LONG	= 414
TEAPOT			= 418
NOT_IMPLEMENTED	= 501

ERROR = 1
localhost = "http://0.0.0.0"
EXIT_CODE = 0

print("Connecting to server...")
# get_from_api(session, path="http://localhost/test")
r = requests.get(localhost)
print("Request send!")

if r.status_code != OK:
    print(f"{Colors.FAILRED}[KO] {Colors.NATURAL} Get request on http://localhost:80")
    EXIT_CODE = 1;
else:
    print(f"{Colors.OKGREEN}[OK] {Colors.NATURAL} Get request on http://localhost:80")

# sleep so that the exit code is that of the python script and not the server
time.sleep(1)
exit(EXIT_CODE)
# p10k


