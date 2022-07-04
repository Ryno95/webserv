import cgi, cgitb
import os
import sys

cgitb.enable()

input_data = cgi.FieldStorage()

print("<!DOCTYPE html>\n<html>\n")
print()
print("<h1>Hello World!</h1>")

# print(input_data.getvalue("name"))
print("</html>\n")
