import socket
import sys

HOST = "127.0.0.1"  # The server's hostname or IP address
PORT = 8080  # The port used by the server

with socket.socket(socket.AF_INET6, socket.SOCK_STREAM) as client:
    client.connect((HOST, PORT))
    client.sendall(b"Wazuuuup")
    data = client.recv(1024)

print(f"Received {data!r}")
print("PRINTING FROM THE CLIENT")

# client REQUEST -> server READ -> server ECHO BACK RESPONSE CODE AND MESSAGE  # Later echo back the body