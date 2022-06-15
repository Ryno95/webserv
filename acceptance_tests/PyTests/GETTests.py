import time

import Request
from defines import Colors
from defines import returnStatus
from defines import Methods
from defines import HttpResponseStatus


EXIT_CODE = 0

localhost = "http://localhost:8080/index.html"
GetRequest = Request.GETRequest(localhost, HttpResponseStatus.OK)
EXIT_CODE += GetRequest.doRequest()

localhost = "http://localhost:8080/applesauce"
GetRequest = Request.GETRequest(localhost, HttpResponseStatus.NOT_FOUND)
EXIT_CODE += GetRequest.doRequest()

time.sleep(0.2)

exit(EXIT_CODE)
