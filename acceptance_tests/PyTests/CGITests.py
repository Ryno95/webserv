import requests
from requests import Session
from requests.structures import CaseInsensitiveDict

import time

from defines import Colors, returnStatus, Methods, HttpResponseStatus, LOCAL_HOST
from Request import Request, CgiRequest
import os

EXIT_CODE = 0

localhost = "http://localhost:8080/cgi-bin/add.py"

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
EXIT_CODE += cgiRequest.compareActualToExpected(HttpResponseStatus.OK, cgiRequest._response.status_code, "Status Code")
if not cgiRequest._response.text.find(_data["val1"] + _data["val2"]):
        EXIT_CODE += 1


localhost = "http://localhost:8080/cgi-bin/NonExistantScript.py"
data = None
cgiRequest = CgiRequest(localhost, data=_data)
cgiRequest.doRequest()
EXIT_CODE += cgiRequest.compareActualToExpected(HttpResponseStatus.NOT_FOUND, cgiRequest._response.status_code, "Status Code")


localhost = "http://localhost:8080/scriptInNonPostBlock.py"
data = None
cgiRequest = CgiRequest(localhost, data=_data)
cgiRequest.doRequest()
EXIT_CODE += cgiRequest.compareActualToExpected(HttpResponseStatus.METHOD_NOT_ALLOWED, cgiRequest._response.status_code, "Status Code")


#  creatign file with only execution rights, needs write permissions to be executable
fd = open(os.open("../../root/cgi-bin/scriptWithWrongRights.py", os.O_CREAT, 0o100), "w")
localhost = "http://localhost:8080/cgi-bin/scriptWithWrongRights.py"
data = None
cgiRequest = CgiRequest(localhost, data=_data)
cgiRequest.doRequest()
EXIT_CODE += cgiRequest.compareActualToExpected(HttpResponseStatus.FORBIDDEN, cgiRequest._response.status_code, "Status Code")
os.remove("../../root/cgi-bin/scriptWithWrongRights.py")

localhost = "http://localhost:8080/cgi-bin/hello.py"
data = None
cgiRequest = CgiRequest(localhost, data=_data)
cgiRequest.doRequest()
EXIT_CODE += cgiRequest.compareActualToExpected(HttpResponseStatus.OK, cgiRequest._response.status_code, "Status Code")
contentLength = cgiRequest._response.headers.get('Content-Length')
# Expecting contentLength for "<!DOCTYPE html>\n<html>\n<h1>Hello World!</h1></html>\n" = 55
EXIT_CODE += cgiRequest.compareActualToExpected(contentLength, "55", "Content-Length")

localhost = "http://localhost:8080/cgi-bin/bigAssScript.py"
data = None
cgiRequest = CgiRequest(localhost, data=_data)
cgiRequest.doRequest()
contentLength = cgiRequest._response.headers.get('Content-Length')
EXIT_CODE += cgiRequest.compareActualToExpected(HttpResponseStatus.OK, cgiRequest._response.status_code, "Status Code")
EXIT_CODE += cgiRequest.compareActualToExpected(contentLength, "5070000", "Content-Length")


exit(EXIT_CODE)
