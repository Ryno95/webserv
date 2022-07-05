import cgi, cgitb
import os
import sys

# cgitb.enable()
# cgi.test()/

# for key in os.environ.keys():
#     print(f'{key}={os.environ[key]}')
# print("BEFORE FieldStorage", file=sys.stderr)sys0s.close
# sys.stdin.close()
# form = cgi.FieldStorage(fp=self.rfile,
#                         headers=self.headers,
#                         environ={"REQUEST_METHOD":op.upper(),
#                               "CONTENT_TYPE":self.headers['Content-Type'],})
# contentLen ="CONTENT_LENGTH=" + os.environ["CONTENT_LENGTH"]
# queryString ="QUERY_STRING=" + os.environ["QUERY_STRING"]
# print(contentLen)
# print(queryString)
# contentLen = os.environ["CONTENT_LENGTH"]
try:
    # input_data = cgi.FieldStorage()
    print("BEFORE RDLINE")
    for line in sys.stdin:
        print(f"Line: {line}");
    print("AFTER RDLINE")
    # for key in input_data.keys():
        #  print(f"{input_data[ key ]} = {input_data[ key ].value}")
except Exception as e:
    print("I dont understand python for shit", file=sys.stderr)
    print(e)
    raise SystemExit(0)


print("Content-Type: text/html") # HTML is following
print()                         # Leave a blank line
print("<!DOCTYPE html>\n<html>\n<body>")
print("<h1>Addition Results</h1>")
try:
    num1 = int(input_data.getvalue("val1"))
    num2 = int(input_data.getvalue("val2"))
except:
    print("<output>Sorry, the script cannot turn your inputs into numbers (integers).</output>\n</body>\n</html>")
    raise SystemExit(0)


print("<output>{0} + {1} = {2}</output>".format(num1, num2, num1 + num2))
