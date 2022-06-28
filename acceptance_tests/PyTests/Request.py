import requests
import os

from pathlib import Path

# Multi import from the same module
from defines import Colors, returnStatus, Methods, HttpResponseStatus, LOCAL_HOST, TEST_FILES_DIR

#Initilize all class vars in __init__
class Request:
    def __init__(self, method: Methods):
        self._method = method

    def printSucces(self, expected, actual, testName):
        print(f"{Colors.OK_GREEN}[OK] {Colors.NATURAL}{testName}\n\tExpected: {repr(expected)}\n\tActual  : {repr(actual)}")

    def printError(self, expected, actual, testName = None):
        print(f"{Colors.FAILED_RED}[KO] {Colors.NATURAL}{testName} \n\tExpected: {repr(expected)}\n\tActual  : {repr(actual)}")
        return returnStatus.ERROR

    def compareActualToExpected(self, expected, actual, testName):
        if not actual.__eq__(expected):
            return self.printError(expected, actual, testName)
        else:
            self.printSucces(expected, actual, testName)
            return returnStatus.SUCCESS

    def printRequest(self, response: requests):
        print(f"\n{Colors.BLUE}{self._method.name} request on {response.url}{Colors.NATURAL}")

    # virtual function in python, throw an error if not implemented in child class
    def doRequest():
        raise NotImplementedError



#  ------------ GET Class definition -----------------
class GETRequest(Request):
    def __init__(self,  uri, expectedHttpCode):
        self._method = Methods.GET
        self._response = []
        self._uri = uri
        self._expected = expectedHttpCode

    def doRequest(self):
        self._response = requests.get(self._uri)
        self.printRequest(self._response)
        return self.compareActualToExpected(self._expected, self._response.status_code, "Status Code")



#  ------------ POST Class definition -----------------
class POSTRequest(Request):
    def __init__(self, fileName=None):
        self._method = Methods.POST
        self._fileName = TEST_FILES_DIR + fileName
        self._fd = 0
        self._body = ""
        self._response = []
        self.createdFile = ""
        self._uri = LOCAL_HOST + "/uploads/" + fileName

    def checkCreatedFileContent(self):
        self.createdFile = self._response.headers.get('Created-file')
        print(self.createdFile)
        if self.createdFile is None:
            self.printError("File Created", "File Not Created", "File Creation")
        self.createdFile = "../../" + self.createdFile
        fileToPostSize = os.stat(self._fileName).st_size
        postedFileSize = os.stat(self.createdFile).st_size
        if self.compareActualToExpected(fileToPostSize, postedFileSize, "File size") == returnStatus.SUCCESS:
            return 0
        else:
            return 1

    def removeCreatedFile(self):
        if os.path.exists(self.createdFile):
            os.remove(self.createdFile)

    def checkCreated(self, response: requests):
       return Request.compareActualToExpected(self, HttpResponseStatus.CREATED, response.status_code, "Status Code")

    def getBodyFromFile(self):
        self._fd = open(self._fileName, 'r')
        return self._fd.read()

    def doRequest(self):
        self._body = self.getBodyFromFile()
        self._response = requests.post(self._uri, data=self._body)
        self.printRequest(self._response)


#   def printSucces(self, expected, actual, testName):
#  ------------ Delete Class definition -----------------
class DELETERequest(Request):
    def __init__(self, target):
        self._method = Methods.DELETE
        self._target = target
        self._fullFilePath = "../../root/data/uploads" + self._target
        self._uri = LOCAL_HOST + "/uploads" + target
        self._response = []
        self._fd = None

    def createFileToDelete(self, permissions):
        self._fd = open(os.open(self._fullFilePath, os.O_CREAT, permissions), 'w') 

    def doRequest(self):
        self._response = requests.delete(self._uri)
        self.printRequest(self._response)
    
    def checkIfDeleted(self, expected: HttpResponseStatus):
        if os.path.exists(self._fullFilePath) and expected is HttpResponseStatus.OK:
            self.printError("Exists", "Deleted", "File Status")
            return 1
        else:
            self.printSucces("Deleted", "Deleted", "File Status")
        return 0

    def compareResult(self, expected: HttpResponseStatus):
        exitCode = 0
        exitCode += self.compareActualToExpected(expected, self._response.status_code, "Status Code")
        exitCode += self.checkIfDeleted(expected)
        if os.path.exists(self._fullFilePath) and not os.path.isdir(self._fullFilePath):
            os.remove(self._fullFilePath)
        return exitCode




#  ------------ CGI Class definition -----------------
class CgiRequest(Request):
    def __init__(self, uri, data):
       self._method = Methods.CGI
       self._uri = uri
       self._data = data
       self._response = []
       self._requestOutput = []
    
    def doRequest(self):
        self._response = requests.post(self._uri, data=self._data)
        self.printRequest(self._response)



# ------------ Class to run all methods consecutively --------------
class IntegrateMethods():
    def __init__ (self, target, expectedGetResponseCode):
        self._target = target
        self._fullFilePath = "../../root" + self._target
        self._uri = LOCAL_HOST + "/" + target
        self._postReqest = POSTRequest(self._target)
        self._getRequest = GETRequest(self._uri, expectedGetResponseCode)
        self._deleteRequest = DELETERequest("/" + target)
        self._expectedCode = expectedGetResponseCode


    def runMethods(self):
        EXIT_CODE = 0

        self._postReqest.doRequest()
        EXIT_CODE += self._postReqest.checkCreated(self._postReqest._response)
        # self._postRequest.checkCreatedFile()
        if EXIT_CODE != 0:
            return EXIT_CODE

        EXIT_CODE += self._getRequest.doRequest()
        if EXIT_CODE != 0:
            return EXIT_CODE

        self._deleteRequest.doRequest()
        EXIT_CODE += self._deleteRequest.compareResult(self._expectedCode)
        
        return EXIT_CODE


