import cgi, cgitb
import os
import sys

USERS_FOLDER	= os.getcwd() + "/root/user_system/users/"
USER_PAGES		= os.getcwd() + "/root/user_system/pages/"

print("Register initiated!", file=sys.stderr)
print("CWD: " + os.getcwd(), file=sys.stderr)

cgitb.enable()
input_data = cgi.FieldStorage()


if os.environ.get("REQUEST_METHOD") != "POST":
	print("invalid method", file=sys.stderr)
	print("Status: 405 Method Not Allowed\n")
	exit(0)

username = str(input_data.getvalue("username"))
password = str(input_data.getvalue("password"))
passwordConfirm = str(input_data.getvalue("passwordConfirm"))

if username == "None" or password == "None" or passwordConfirm == "None":
	print("invalid arguments", file=sys.stderr)
	print("Status: 400 Bad Request\n")
	exit(0)

if password != passwordConfirm:
	print("password mismatch: '" + password + "' != '" + passwordConfirm + "'", file=sys.stderr)
	print("Location: " + USER_PAGES + "password_mismatch.html\n")
	exit(0)

if os.path.exists(USERS_FOLDER + username):
	print("user already exists", file=sys.stderr)
	print("Location: " + USER_PAGES + "user_already_exists.html\n")
	exit(0)

print("Creating file: " + USERS_FOLDER + username, file=sys.stderr)
print("Continue!", file=sys.stderr)

file = open(USERS_FOLDER + username, "w")
file.write("password=" + password)
file.close()

print("Location: " + USER_PAGES + "user_created.html\n")
print("SUCCESSSS!!!!", file=sys.stderr)
