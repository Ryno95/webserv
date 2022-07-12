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
	print("User is not logged in yet, and thus cannot show favourites.", file=sys.stderr)
	print("Location: " + USER_PAGES + "login.html\n")
	exit(0)

print("Status: 200 OK\n")
print("<!DOCTYPE html>\n<html>\n<body>")

favouritesCookie = cookies.get("favourites")
if favouritesCookie == None:
	print("<h2>There are no favourites added yet :O</h2>")
	print("<a href=\"/favourites.html\"><button>Let's add some</button></a>")
else:
	favourites = str(favouritesCookie).split("&")
	print("<p>" + cookies.get("user") + " seems to have some weird favourites:</p>\n<ul>")
	for favourite in favourites:
		print("<li>" + favourite + "</li>")
	print("</ul>")

print("</body>\n</html>")
