import time
import os

from defines import Colors, returnStatus, Methods, HttpResponseStatus, LOCAL_HOST
from Request import Request, DELETERequest


EXIT_CODE = 0

#  ----------- Test dir in server to test DELETE method -----------
if not os.path.exists("../../../server/root/data/uploads/deleteFolderTest"):
    os.mkdir("../../../server/root/data/uploads/deleteFolderTest")


target = "/deleteFolderTest/.algoDentro"
deleteRequest = DELETERequest(target)
deleteRequest.createFileToDelete(0o777)
deleteRequest.doRequest()
EXIT_CODE += deleteRequest.compareResult(HttpResponseStatus.OK)

target = "/deleteFolderTest/NonExistant"
deleteRequest = DELETERequest(target)
deleteRequest.doRequest()
EXIT_CODE += deleteRequest.compareResult(HttpResponseStatus.NOT_FOUND)

target = "/deleteFolderTest/OnlyReadRight"
deleteRequest = DELETERequest(target)
deleteRequest.createFileToDelete(0o444)
deleteRequest.doRequest()
EXIT_CODE += deleteRequest.compareResult(HttpResponseStatus.FORBIDDEN)

#  -------- try to delete a folder ----------------
target = "/deleteFolderTest"
deleteRequest = DELETERequest(target)
deleteRequest.doRequest()
EXIT_CODE += deleteRequest.compareResult(HttpResponseStatus.OK)

exit(EXIT_CODE)
