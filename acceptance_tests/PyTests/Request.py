import requests
# Multi import from the same module
from defines import Colors, returnStatus, Methods, HttpResponseStatus, LOCAL_HOST

#Initilize all class vars in __init__
class Request:
    def __init__(self, method: Methods):
        self._method = method

    def printSucces(self):
         print(f"{Colors.OK_GREEN}[OK] {Colors.NATURAL}")

    def printError(self, expected, actual):
        print(f"{Colors.FAILED_RED}[KO] {Colors.NATURAL} Expected: {expected} Actual: {actual}")
        return returnStatus.ERROR

    def checkHttpCode(self, expected, actual):
        if expected != actual:
            return self.printError(expected, actual)
        else:
            return returnStatus.SUCCESS

    def compareExpectedPositiveResult(self, expectedHttpResponse, response: requests):
        testStatus = 0

        print(f"POST request on {response.url}")
        testStatus += self.checkHttpCode(expectedHttpResponse, response.status_code)

        if testStatus == returnStatus.SUCCESS:
            self.printSucces()
        return testStatus

    # virtual function in python, throw an error if not implemented in child class
    def doRequest():
        raise NotImplementedError


class GETRequest(Request):
    pass

class POSTRequest(Request):
    def __init__(self, fileName):
        self._fileName = fileName
        self._fd = 0
        self._body = ""
        self._response = []
        self._uri = LOCAL_HOST + "/" + fileName

    def checkCreated(self, response: requests):
       return Request.compareExpectedPositiveResult(self, HttpResponseStatus.CREATED, response)

    def getBodyFromFile(self):
        self._fd = open(self._fileName, 'r')
        return self._fd.read()

    def doRequest(self):
        self._body = self.getBodyFromFile()
        self._response = requests.post(self._uri, data=self._body)
        # pass
    
class DELETERequest(Request):
    pass