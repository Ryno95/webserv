from requests import Session
from requests.structures import CaseInsensitiveDict
from defines import HttpResponseStatus
from defines import Methods
from defines import HttpResponseStatus

import time
import requests
from Request import Request, POSTRequest

fileNames = ["simplePost.txt"]

EXIT_CODE = 0

PostRequest = POSTRequest("simplePost.txt")
PostRequest.doRequest()
EXIT_CODE += PostRequest.checkCreated(PostRequest._response)

# # sleep so that the exit code is that of the python script and not the server
time.sleep(1)

exit(EXIT_CODE)
