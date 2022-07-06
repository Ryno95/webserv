import os

print("Status: 200 OK")
print("Location: " + os.getcwd() + "/root/server_redirect.html\n")
print("<!DOCTYPE html>\n<html>")

print("<h1>Server redirect</h1>")
print("<h2>This should not be printed</h2>")

print("</html>")
