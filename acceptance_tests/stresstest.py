import socket
import sys
import threading
import random
import os
import time

def connect(msg):
	with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
		s.connect((HOST, PORT))
		s.send(msg.encode())
		time.sleep(1)


HOST = "127.0.0.1"	# Server's hostname or IP address
PORT = 8080			# Port used by the server

fd = open('requests.txt', "r")
requests = fd.readlines()
fd.close()

threadcount = 0
threads = []
for request in requests:
	threads.append(threading.Thread(target=connect, args=(request,)))
	threads[threadcount].start()
	threadcount += 1

for i in range(threadcount):
	threads[i].join()
print("Done with " + str(threadcount) + " requests")
