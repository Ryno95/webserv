import time
import requests


ERROR = 1
localhost = "http://localhost:4242"
EXIT_CODE = 0

r = requests.get(localhost)

print(r.status_code)
if r.status_code != '10':
    EXIT_CODE = 123;
print(r.text)
print(r.headers['content-length'])

time.sleep(3)
exit(EXIT_CODE)
# p10k