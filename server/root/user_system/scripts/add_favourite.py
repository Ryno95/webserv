# Add an item to the favourites cookie.

from ast import Try
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

# Section get cookies
cookieLine = os.environ.get("HTTP_COOKIE")
if cookieLine == None:
	print("No cookies received at all!", file=sys.stderr)
	print("Location: " + USER_PAGES + "login.html\n")
	exit(0)

cookiesArray = cookieLine.split("; ")

cookies = {}
for cookie in cookiesArray:
	pair = cookie.split("=")
	cookies[pair[0]] = pair[1]

for value in cookies:
	print(value, file=sys.stderr)
# End of section cookies

if cookies.get("user") == None:
	print("User is not logged in yet, and thus cannot add favourites.", file=sys.stderr)
	print("Location: " + USER_PAGES + "login.html\n")
	exit(0)

newFavourite = input_data.getvalue("favourite")
if newFavourite == None:
	print("No new favourite item supplied", file=sys.stderr)
	print("Status: 400 Bad Request\n")
	exit(0)

favourites = cookies.get("favourites")
if favourites == None:
	favourites = newFavourite
else:
	favourites += "&" + newFavourite

print("Set-Cookie: favourites=" + favourites)
print("Status: 204 No Content\n")
