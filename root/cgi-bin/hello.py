import cgi, cgitb
import os
import sys

cgitb.enable()
input_data = cgi.FieldStorage()

print("Content-Type: text/html")
print()
print("<!DOCTYPE html>\n<html>")
print("<h1>Hello World!</h1>")

print("</html>")
