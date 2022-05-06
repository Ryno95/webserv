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

time.sleep(0.5)
PostRequest = POSTRequest(basicFile)
PostRequest.doRequest()
EXIT_CODE += PostRequest.checkCreated(PostRequest._response)
PostRequest.checkCreatedFile()
# PostRequest.cleanUp()
time.sleep(0.5)

time.sleep(0.5)
PostRequest = POSTRequest(bigFile)
PostRequest.doRequest()
EXIT_CODE += PostRequest.checkCreated(PostRequest._response)
PostRequest.checkCreatedFile()
# PostRequest.cleanUp()
time.sleep(0.5)


time.sleep(0.5)
PostRequest = POSTRequest(emptyFile)
PostRequest.doRequest()
EXIT_CODE += PostRequest.checkCreated(PostRequest._response)
PostRequest.checkCreatedFile()
# PostRequest.cleanUp()
time.sleep(0.5)
# # sleep so that the exit code is that of the python script and not the server
time.sleep(1)

exit(EXIT_CODE)
