import requests
from requests import Session
from requests.structures import CaseInsensitiveDict

import time

from defines import Colors, returnStatus, Methods, HttpResponseStatus, LOCAL_HOST
from Request import Request, POSTRequest

bigFile = "bigPost.txt"
basicFile = "simplePost.txt"
emptyFile = "emptyPost.txt"

EXIT_CODE = 0

PostRequest = POSTRequest(basicFile)
PostRequest.doRequest()
EXIT_CODE += PostRequest.checkCreated(PostRequest._response)
PostRequest.checkCreatedFile()

PostRequest = POSTRequest(bigFile)
PostRequest.doRequest()
EXIT_CODE += PostRequest.checkCreated(PostRequest._response)
PostRequest.checkCreatedFile()


PostRequest = POSTRequest(emptyFile)
PostRequest.doRequest()
EXIT_CODE += PostRequest.checkCreated(PostRequest._response)
PostRequest.checkCreatedFile()

# # sleep so that the exit code is that of the python script and not the server
time.sleep(0.2)

exit(EXIT_CODE)
