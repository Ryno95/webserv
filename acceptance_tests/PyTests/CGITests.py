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

cgiRequest = CgiRequest(localhost, data=_data)
try:
    cgiRequest.doRequest()
except Exception as e:
    print("CGI REQUEST FAILED: ")
    print(e)
    raise SystemExit(69)
EXIT_CODE += cgiRequest.compareExpectedPositiveResult(HttpResponseStatus.OK, cgiRequest._response)
if not cgiRequest._response.text.find(_data["val1"] + _data["val2"]):
        EXIT_CODE += 1



localhost = "http://localhost:8080/NonExistantScript.py"
# Create query string QUERY_STRING=val1=6&val2=63
_data={"val1":"6",
        "val2":"63"}

cgiRequest = CgiRequest(localhost, data=_data)
try:
    cgiRequest.doRequest()
except Exception as e:
    print("CGI REQUEST FAILED: ")
    print(e)
    raise SystemExit(69)
EXIT_CODE += cgiRequest.compareExpectedPositiveResult(HttpResponseStatus.NOT_FOUND, cgiRequest._response)
if not cgiRequest._response.text.find(_data["val1"] + _data["val2"]):
        EXIT_CODE += 1


exit(EXIT_CODE)