import time
import requests
import Request

from defines import Colors
from defines import returnStatus
from defines import Methods
from defines import HttpResponseStatus


EXIT_CODE = 0

localhost = "http://localhost:8080"
GetRequest = Request.GETRequest()
response  = requests.get(localhost)
EXIT_CODE += GetRequest.compareExpectedPositiveResult(HttpResponseStatus.OK, response)

localhost = "http://localhost:8080/applesauce"
GetRequest = Request.GETRequest()
response  = requests.get(localhost)
EXIT_CODE += GetRequest.compareExpectedPositiveResult(HttpResponseStatus.NOT_FOUND, response)

localhost = "http://localhost:8080/hello.py"
basicCgiResponseBody = "<!DOCTYPE html>\n<html>\n\n<h1>Hello World!</h1>\n</html>\n\n"
GetRequest = Request.GETRequest()
response  = requests.get(localhost)
EXIT_CODE += GetRequest.compareExpectedPositiveResult(HttpResponseStatus.OK, response)
EXIT_CODE += GetRequest.compareActualToExpected(response.text, basicCgiResponseBody)
time.sleep(1)

exit(EXIT_CODE)
