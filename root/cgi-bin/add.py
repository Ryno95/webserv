import cgi, cgitb
import os

cgitb.enable()

form = cgi.FieldStorage()

print('Content-Type: text/html') # HTML is following
print('')                         # Leave a blank line
print('<h1>Addition Results</h1>')
try:
    num1 = int(form["val1"].value)
    num2 = int(form["val2"].value)
except:
    print('<output>Sorry, the script cannot turn your inputs into numbers (integers).</output>')
    raise SystemExit(1)
print('<output>{0} + {1} = {2}</output>'.format(num1, num2, num1 + num2))



# print("<TITLE>CGI script output</TITLE>")
# print("<H1>This is my first CGI script</H1>")
# print("Hello, world!")