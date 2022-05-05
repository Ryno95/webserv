from requests import Session
from requests.structures import CaseInsensitiveDict
from defines import HttpResponseStatus
from defines import Methods
from defines import HttpResponseStatus

import time
import requests
import Request

EXIT_CODE = 0

url = "http://localhost:8080/testies.txt"
_headers = CaseInsensitiveDict()
_headers["Accept"] = "text/plain"
_headers["Content-Type"] = "text/plain"
# _headers["Content-Length"] = "24"

_data = "What is up vanilla face!"

PostRequest = Request.POSTRequest(Methods.POST);

response = requests.post(url, headers=_headers, data=_data)

EXIT_CODE += PostRequest.checkCreated(response)

# # sleep so that the exit code is that of the python script and not the server
time.sleep(1)

exit(EXIT_CODE)
