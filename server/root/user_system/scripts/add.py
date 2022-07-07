# Add an item to the favourites cookie.

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


cookieLine = os.environ.get("HTTP_COOKIE")
if cookieLine == None:
	print("No cookies received at all!", file=sys.stderr)
	print("Location: " + USER_PAGES + "login.html\n")
	exit(0)

print("Current cookies received: " + cookieLine, file=sys.stderr)

cookies = cookieLine.split("; ")

print("Made it though", file=sys.stderr)
print(cookies, file=sys.stderr)

currentFavourites = ""
found = False
for cookie in cookies:
	if cookie.startswith("user="):
		found = True
	if cookie.startswith("favourites="):
		currentFavourites = cookie[11:]

if found == False:
	print("User is not logged in yet, and thus cannot add favourites.", file=sys.stderr)
	print("Location: " + USER_PAGES + "login.html\n")
	exit(0)

newFavourite = input_data.getvalue("favourite")
print("New: " + newFavourite, file=sys.stderr)

if newFavourite == None:
	print("No new favourite item supplied", file=sys.stderr)
	print("Status: 400 Bad Request\n")
	exit(0)

if currentFavourites != "":
	currentFavourites += "&"
currentFavourites += newFavourite

print("Set-Cookie: favourites=" + currentFavourites)
print("Status: 204 No Content\n")
