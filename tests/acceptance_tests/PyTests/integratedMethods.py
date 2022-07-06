import requests
from requests import Session
from requests.structures import CaseInsensitiveDict

import time
import os

from defines import Colors, returnStatus, Methods, HttpResponseStatus, LOCAL_HOST
from Request import Request, GETRequest, POSTRequest ,DELETERequest, IntegrateMethods


# IntegrateMethods(target, expectedGetResponse):
# IntgratedMethods all expect the same return code, so only good actually for OK responses
# Target should NOT start with a forward slash as appending happens internally
EXIT_CODE = 0

target = "simplePost.txt"
im = IntegrateMethods(target, HttpResponseStatus.OK)
EXIT_CODE += im.runMethods()

exit(EXIT_CODE)