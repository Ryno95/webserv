from urllib import response
import requests
from requests import Session
from requests.structures import CaseInsensitiveDict

import time

from defines import Colors, returnStatus, Methods, HttpResponseStatus, LOCAL_HOST
from Request import Request, CgiRequest

EXIT_CODE = 0

localhost = "http://localhost:8080/add.py"

# Create query string QUERY_STRING=val1=6&val2=63
_data={"val1":"6",
        "val2":"63"}

# cgiRequest = CgiRequest(localhost, data=_data)
# cgiRequest.doRequest()
response = requests.post(localhost, data=_data) 
if (response.status_code != HttpResponseStatus.OK):
    EXIT_CODE += 1
else:
    print("OK!")
# EXIT_CODE += cgiRequest.compareExpectedPositiveResult(HttpResponseStatus.OK, cgiRequest._response)
if not response.text.find(_data["val1"] + _data["val2"]):
        EXIT_CODE += 1
else:
    print("OK!")
exit(EXIT_CODE)