import requests
from defines import Colors
from defines import returnStatus
from defines import Methods


class Request:
    def __init__(self, method: Methods):
        self._method = method
    
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

        print(f"{Colors.BLUE}POST request on {response.url} {Colors.NATURAL}")
        testStatus += self.checkHttpCode(expectedHttpResponse, response.status_code)
        return testStatus


class GETRequest(Request):
    pass

class POSTRequest(Request):
    pass