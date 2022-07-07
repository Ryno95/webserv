# Create a page containing all the favourite items from the cookie sent by the client.

import cgi, cgitb
import os
import sys

USERS_FOLDER	= os.getcwd() + "/root/user_system/users/"
USER_PAGES		= os.getcwd() + "/root/user_system/pages/"

cgitb.enable()
input_data = cgi.FieldStorage()

if os.environ.get("REQUEST_METHOD") != "POST":
	print("invalid method", file=sys.stderr)
	print("Status: 405 Method Not Allowed\n")
	exit(0)

currentCookie = os.environ.get("HTTP_COOKIE")

if currentCookie == None or not currentCookie.__contains__("user="):
	print("User is not logged in yet, and thus cannot pick favourites.", file=sys.stderr)
	print("Location: " + USER_PAGES + "login.html\n")
	exit(0)

print(currentCookie, file=sys.stderr)

# while True:
# 	pos = currentCookie.find(";")
# 	if pos == -1:
# 		break

print("Status: 200 OK\n")
