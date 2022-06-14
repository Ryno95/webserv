import requests
import os

from pathlib import Path

# Multi import from the same module
from defines import Colors, returnStatus, Methods, HttpResponseStatus, LOCAL_HOST

#Initilize all class vars in __init__
class Request:
    def __init__(self, method: Methods):
        self._method = method

    def printSucces(self, expected, actual):
        print(f"{Colors.OK_GREEN}[OK] {Colors.NATURAL} Expected: {repr(expected)} Actual  : {repr(actual)}")

    def printKo(slef, optionalMessage):
        if optionalMessage:
            print(f"{Colors.FAILED_RED}[KO] {Colors.NATURAL} {optionalMessage}")
        else:
            print(f"{Colors.FAILED_RED}[KO] {Colors.NATURAL}")

    def printOk(self, optionalMessage):
        if optionalMessage:
            print(f"{Colors.OK_GREEN}[OK] {Colors.NATURAL} {optionalMessage}")
        else:
            print(f"{Colors.OK_GREEN}[OK] {Colors.NATURAL}")

    def printError(self, expected, actual):
        print(f"{Colors.FAILED_RED}[KO] {Colors.NATURAL}\nExpected: {repr(expected)} Actual  : {repr(actual)}")
        return returnStatus.ERROR

    def compareActualToExpected(self, expected, actual):
        if not actual.__eq__(expected):
            return self.printError(expected, actual)
        else:
            return returnStatus.SUCCESS

    def printRequest(self, response: requests):
        print(f"\n{self._method.name} request on {response.url}")

    def compareExpectedPositiveResult(self, expectedHttpResponse, response: requests):
        testStatus = 0

        self.printRequest(response)
        testStatus += self.compareActualToExpected(expectedHttpResponse, response.status_code)

        if testStatus == returnStatus.SUCCESS:
            self.printSucces(expectedHttpResponse, response.status_code)
        return testStatus

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
        return self.compareExpectedPositiveResult(self._expected, self._response)



#  ------------ POST Class definition -----------------
class POSTRequest(Request):
    def __init__(self, fileName=None):
        self._method = Methods.POST
        self._fileName = fileName
        self._fd = 0
        self._body = ""
        self._response = []
        self._uri = LOCAL_HOST + "/" + fileName

    def checkCreatedFile(self):
        createdFile = self._response.headers.get('Created-file')
        if createdFile is None:
            self.printError(self._body, createdFile)
        filename = "".join(("../../", str(createdFile)))
        fd = open(filename, 'r')
        if self.compareActualToExpected(fd.read(), self._body) == returnStatus.SUCCESS:
            self.printOk("Posted entire body")
        if os.path.exists(filename):
            os.remove(filename)

    def checkCreated(self, response: requests):
       return Request.compareExpectedPositiveResult(self, HttpResponseStatus.CREATED, response)

    def getBodyFromFile(self):
        self._fd = open(self._fileName, 'r')
        return self._fd.read()

    def doRequest(self):
        self._body = self.getBodyFromFile()
        self._response = requests.post(self._uri, data=self._body)



#  ------------ Delete Class definition -----------------
class DELETERequest(Request):
    def __init__(self, target):
        self._method = Methods.DELETE
        self._target = target
        self._fullFilePath = "../../root" + self._target
        self._uri = LOCAL_HOST + target
        self._response = []
        self._fd = None

    def createFileToDelete(self, permissions):
        self._fd = open(os.open(self._fullFilePath, os.O_CREAT, permissions), 'w') 

    def doRequest(self):
        self._response = requests.delete(self._uri)
    
    def checkIfDeleted(self, expected: HttpResponseStatus):
        if os.path.exists(self._fullFilePath) and expected is HttpResponseStatus.OK:
            self.printKo("File should be deleted")
            return 1
        else:
            self.printOk("File doesn't exist anymore")
        return 0

    def compareResult(self, expected: HttpResponseStatus):
        exitCode = 0
        exitCode += self.compareExpectedPositiveResult(expected, self._response)
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


