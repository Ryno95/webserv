import time
import requests
from enum import Enum

class Colors:
    OKGREEN =   '\033[92m'
    FAILRED =       '\033[91m'
    NATURAL =   '\033[0m'

OK				= 200
BAD_REQUEST		= 400
NOT_FOUND		= 404
URI_TOO_LONG	= 414
TEAPOT			= 418
NOT_IMPLEMENTED	= 501

ERROR = 1
localhost = "http://localhost:4242"
EXIT_CODE = 0


r = requests.get(localhost)

if r.status_code != 200:
    print(f"{Colors.FAILRED}[KO] {Colors.NATURAL} Get request on http://localhost:4242")
    EXIT_CODE = 1;
else:
    print(f"{Colors.OKGREEN}[OK] {Colors.NATURAL} Get request on http://localhost:4242")

time.sleep(3)
exit(EXIT_CODE)
# p10k


