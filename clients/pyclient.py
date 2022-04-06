import requests

x = requests.get('http://127.0.0.1:8080')

print(x.text)