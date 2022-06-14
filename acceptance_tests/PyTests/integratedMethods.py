import requests
from requests import Session
from requests.structures import CaseInsensitiveDict

import time
import os

from defines import Colors, returnStatus, Methods, HttpResponseStatus, LOCAL_HOST
from Request import Request, GETRequest, POSTRequest ,DELETERequest, IntegrateMethods

EXIT_CODE = 0

# IntegrateMethods(target, expectedGetResponse)
target = "simplePost.txt"
im = IntegrateMethods(target, HttpResponseStatus.OK)
EXIT_CODE += im.runMethods()

exit(EXIT_CODE)