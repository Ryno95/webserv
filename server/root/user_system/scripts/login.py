import cgi, cgitb
import os
import sys

USERS_FOLDER	= os.getcwd() + "/root/user_system/users/"
USER_PAGES		= os.getcwd() + "/root/user_system/pages/"

print("Login initiated!", file=sys.stderr)
print("CWD: " + os.getcwd(), file=sys.stderr)

cgitb.enable()
input_data = cgi.FieldStorage()

if os.environ.get("REQUEST_METHOD") != "POST":
	print("invalid method", file=sys.stderr)
	print("Status: 405 Method Not Allowed\n")
	exit(0)

usernameInput = str(input_data.getvalue("username"))
passwordInput = str(input_data.getvalue("password"))

if usernameInput == "None" or passwordInput == "None":
	print("invalid arguments", file=sys.stderr)
	print("Status: 400 Bad Request\n")
	exit(0)

if not os.path.exists(USERS_FOLDER + usernameInput):
	print("user does not exist", file=sys.stderr)
	print("Location: " + USER_PAGES + "invalid_login.html\n")
	exit(0)

password = None

file = open(USERS_FOLDER + usernameInput, "r")
lines = file.readlines()
for line in lines:
	pos = line.find("=")
	if line.startswith("password="):
		password = line[pos + 1:]

file.close()

if password == None:
	print("No password found in user file", file=sys.stderr)

print("password from file: " + password, file=sys.stderr)

if password != passwordInput:
	print("Invalid password supplied", file=sys.stderr)
	print("Location: " + USER_PAGES + "invalid_login.html\n")
	exit(0)

file = open(USERS_FOLDER + usernameInput, "w")
file.write("password=" + password)
file.close()

print("Set-Cookie: user=" + usernameInput)
print("Location: " + USER_PAGES + "logged_in.html")
print("Status: 200 OK\n")
