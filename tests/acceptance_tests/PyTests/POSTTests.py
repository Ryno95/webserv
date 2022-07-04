import time

from defines import Colors, returnStatus, Methods, HttpResponseStatus, LOCAL_HOST
from Request import Request, POSTRequest

# PostRequest expects the path to a file
# checkCreated() method will check that the return code == 201
# checkCreatedFileContent() will check that the file to post and the posted file has the same contents

bigFile = "bigPost.txt"
basicFile = "simplePost.txt"
singleLineFile = "singleLineFile.txt"
emptyFile = "emptyFile.txt"

EXIT_CODE = 0

postRequest = POSTRequest(basicFile)
postRequest.doRequest()
EXIT_CODE += postRequest.checkCreated(postRequest._response)
postRequest.checkCreatedFileContent()
postRequest.removeCreatedFile()

postRequest = POSTRequest(bigFile)
postRequest.doRequest()
EXIT_CODE += postRequest.checkCreated(postRequest._response)
postRequest.checkCreatedFileContent()
postRequest.removeCreatedFile()


postRequest = POSTRequest(singleLineFile)
postRequest.doRequest()
EXIT_CODE += postRequest.checkCreated(postRequest._response)
postRequest.checkCreatedFileContent()
postRequest.removeCreatedFile()

postRequest = POSTRequest(emptyFile)
postRequest.doRequest()
EXIT_CODE += postRequest.checkCreated(postRequest._response)
postRequest.checkCreatedFileContent()
postRequest.removeCreatedFile()

# # sleep so that the exit code is that of the python script and not the server
time.sleep(0.2)

exit(EXIT_CODE)
