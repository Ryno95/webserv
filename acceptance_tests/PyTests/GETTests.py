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

# # sleep so that the exit code is that of the python script and not the server
time.sleep(1)

exit(EXIT_CODE)



