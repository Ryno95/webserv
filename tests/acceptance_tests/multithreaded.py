import requests
import threading

def get():
	print("Get request sent")
	x = requests.get('http://127.0.0.1:8080')
	print(x.text)

threadcount = 0
threads = []
for i in range(2):
	threads.append(threading.Thread(target=get))
	threads[threadcount].start()
	threadcount += 1

for i in range(threadcount):
	threads[i].join()
